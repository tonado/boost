# Status: ported, except for tests and --abbreviate-paths.
# Base revision: 40480
#
# Copyright 2001, 2002, 2003 Dave Abrahams 
# Copyright 2006 Rene Rivera 
# Copyright 2002, 2003, 2004, 2005, 2006 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import re
from b2.util.utility import *
from b2.build import feature
from b2.util import sequence, set

__re_two_ampersands = re.compile ('&&')
__re_comma = re.compile (',')
__re_split_condition = re.compile ('(.*):(<.*)')
__re_toolset_feature = re.compile ('^(<toolset>|<toolset->)')
__re_os_feature = re.compile ('^(<os>)')
__re_split_conditional = re.compile (r'(.+):<(.+)')
__re_colon = re.compile (':')
__re_has_condition = re.compile (r':<')
__re_separate_condition_and_property = re.compile (r'(.*):(<.*)')

def reset ():
    """ Clear the module state. This is mainly for testing purposes.
    """
    global __results

    # A cache of results from as_path
    __results = {}
    
reset ()

        
def path_order (x, y):
    """ Helper for as_path, below. Orders properties with the implicit ones
        first, and within the two sections in alphabetical order of feature
        name.
    """
    if x == y:
        return 0
        
    xg = get_grist (x)
    yg = get_grist (y)

    if yg and not xg:
        return -1

    elif xg and not yg:
        return 1

    else:
        if not xg:            
            x = feature.expand_subfeatures([x])
            y = feature.expand_subfeatures([y])
        
        if x < y:
            return -1
        elif x > y:
            return 1
        else:
            return 0

def abbreviate_dashed(string):
    # FIXME: string.abbreviate?
    return [string.abbreviate(part) for part in string.split('-')].join('-')

def identify(string):
    return string 

# FIXME: --abbreviate-paths        
    
def as_path (properties):
    """ Returns a path which represents the given expanded property set.
    """
    key = '-'.join (properties)
    
    if not __results.has_key (key):
        # trim redundancy
        properties = feature.minimize (properties)
    
        # sort according to path_order
        properties.sort (path_order)
    
        components = []
        for p in properties:
            pg = get_grist (p)
            # FIXME: abbrev?
            if pg:
                f = ungrist (pg)
                components.append (f + '-' + replace_grist (p, ''))

            else:
                components.append (p)
        
        __results [key] = '/'.join (components)
    
    return __results [key]
    
def refine (properties, requirements):
    """ Refines 'properties' by overriding any non-free properties 
        for which a different value is specified in 'requirements'. 
        Conditional requirements are just added without modification.
        Returns the resulting list of properties.
    """
    # The result has no duplicates, so we store it in a map
    # TODO: use a set from Python 2.4?
    result = {}
    
    # Records all requirements.
    required = {}
    
    # All the elements of requirements should be present in the result
    # Record them so that we can handle 'properties'.
    for r in requirements:
        # Don't consider conditional requirements.
        if not is_conditional (r):
            # Note: cannot use local here, so take an ugly name
            required [get_grist (r)] = replace_grist (r, '')

    for p in properties:
        # Skip conditional properties
        if is_conditional (p):
            result [p] = None
        # No processing for free properties
        elif 'free' in feature.attributes (get_grist (p)):
            result [p] = None
        else:
            if required.has_key (get_grist (p)):
                required_value = required [get_grist (p)]
                
                value = replace_grist (p, '')

                if value != required_value:
                    result [replace_grist (required_value, get_grist (p))] = None
                else:
                    result [p] = None
            else:
                result [p] = None

    return result.keys () + requirements

def translate_paths (properties, path):
    """ Interpret all path properties in 'properties' as relative to 'path'
        The property values are assumed to be in system-specific form, and
        will be translated into normalized form.
        """
    result = []

    for p in properties:
        split = split_conditional (p)

        condition = ''

        if split:
            condition = split [0]
            p = split [1]
        
        if get_grist (p) and 'path' in feature.attributes (get_grist (p)):
            values = __re_two_ampersands.split (forward_slashes (get_grist (p)))

            t = [os.path.join(path, v) for v in values]
            t = '&&'.join (t)
            tp = replace_grist (t, get_grist (p)).replace("\\", "/")
            result.append (condition + tp)
            
        else:
            result.append (condition + p)

    return result

def translate_indirect(specification, context_module):
    """Assumes that all feature values that start with '@' are
    names of rules, used in 'context-module'. Such rules can be
    either local to the module or global. Qualified local rules
    with the name of the module."""
    result = []
    for p in specification:
        if p[0] == '@':
            m = p[1:]
            if not '.' in p:
                # This is unqualified rule name. The user might want
                # to set flags on this rule name, and toolset.flag
                # auto-qualifies the rule name. Need to do the same
                # here so set flag setting work.
                # We can arrange for toolset.flag to *not* auto-qualify
                # the argument, but then two rules defined in two Jamfiles
                # will conflict.
                m = context_module + "." + m

            result.append(get_grist(p) + "@" + m)
        else:
            result.append(p)

    return result

def validate (properties):
    """ Exit with error if any of the properties is not valid.
        properties may be a single property or a sequence of properties.
    """
    
    if isinstance (properties, str):
        __validate1 (properties)
    else:
        for p in properties:
            __validate1 (p)

def expand_subfeatures_in_conditions (properties):

    result = []
    for p in properties:
        s = __re_split_condition.match (p)

        if not s:
            result.append (p)

        else:
            condition = s.group (1)

            # Condition might include several elements
            condition = __re_comma.split (condition)

            value = s.group (2)

            e = []
            for c in condition:

                cg = get_grist (c)
                if __re_toolset_feature.match (cg) or __re_os_feature.match (cg):
                    # It common that condition includes a toolset which
                    # was never defined, or mentiones subfeatures which
                    # were never defined. In that case, validation will
                    # only produce an spirious error, so don't validate.
                    e.append (feature.expand_subfeatures (c, True))

                else:
                    e.append (feature.expand_subfeatures (c))
            
            if e == condition:
                result.append (p)

            else:
                individual_subfeatures = Set.difference (e, condition)
                result.append (','.join (individual_subfeatures) + ':' + value)

    return result

def make (specification):
    """ Converts implicit values into full properties.
    """
    result = []
    for e in specification:
        if get_grist (e):
            result.append (e)

        elif feature.is_implicit_value (e):
            f = feature.implied_feature (e)
            result.append (f + e)

        else:
            raise InvalidProperty ("'%s' is not a valid for property specification" % e)

    return result


def split_conditional (property):
    """ If 'property' is conditional property, returns
        condition and the property, e.g
        <variant>debug,<toolset>gcc:<inlining>full will become
        <variant>debug,<toolset>gcc <inlining>full.
        Otherwise, returns empty string.
    """
    m = __re_split_conditional.match (property)
    
    if m:
        return (m.group (1), '<' + m.group (2))

    return None

def is_conditional (property):
    """ Returns True if a property is conditional.
    """
    if __re_colon.search (replace_grist (property, '')):
        return True
    else:
        return False

def select (features, properties):
    """ Selects properties which correspond to any of the given features.
    """
    result = []
    
    # add any missing angle brackets
    features = add_grist (features)

    return [p for p in properties if get_grist(p) in features]

def validate_property_sets (sets):
    for s in sets:
        validate(feature.split(s))


def evaluate_conditionals_in_context (properties, context):
    """ Removes all conditional properties which conditions are not met
        For those with met conditions, removes the condition. Properies
        in conditions are looked up in 'context'
    """
    base = []
    conditionals = []

    for p in properties:
        if __re_has_condition.search (p):
            conditionals.append (p)
        else:
            base.append (p)

    result = base
    for p in conditionals:

        # Separate condition and property
        s = __re_separate_condition_and_property.match (p)

        # Split condition into individual properties
        conditions = s.group (1).split (',')

        # Evaluate condition        
        if set.contains (c, context):
            result.append (s.group (2))

    return result

def expand_subfeatures_in_conditions(properties):

    result = []
    for p in properties:

        s = __re_separate_condition_and_property.match(p)
        if not s:
            result.append(p)
        else:
            condition = s.group(1)
            # Condition might include several elements
            condition = condition.split(",")
            value = s.group(2)
            
            e = []
            for c in condition:
                # It common that condition includes a toolset which
                # was never defined, or mentiones subfeatures which
                # were never defined. In that case, validation will
                # only produce an spirious error, so prevent
                # validation by passing 'true' as second parameter.
                e.extend(feature.expand_subfeatures(c, dont_validate=True))

            if e == condition:
                result.append(p)
            else:
                individual_subfeatures = set.difference(e, condition)
                result.append(",".join(individual_subfeatures) + ":" + value)
                
    return result

def change (properties, feature, value = None):
    """ Returns a modified version of properties with all values of the
        given feature replaced by the given value.
        If 'value' is None the feature will be removed.
    """
    result = []
    
    feature = add_grist (feature)

    for p in properties:
        if get_grist (p) == feature:
            if value:
                result.append (replace_grist (value, feature))

        else:
            result.append (p)

    return result


################################################################
# Private functions

def __validate1 (property):
    """ Exit with error if property is not valid.
    """        
    msg = None

    f = get_grist (property)
    if f:
        value = get_value (property)

        if not feature.valid (f):
            f = ungrist (get_grist (property)) # Ungrist for better error messages
            msg = "Unknown feature '%s'" % f

        elif value and not 'free' in feature.attributes (f):
            feature.validate_value_string (f, value)

        elif not value:
            f = ungrist (get_grist (property)) # Ungrist for better error messages
            msg = "No value specified for feature '%s'" % f

    else:
        f = feature.implied_feature (property)
        feature.validate_value_string (f, property)

    if msg:
        # FIXME: don't use globals like this. Import here to
        # break circular dependency.
        from b2.manager import get_manager
        get_manager().errors()("Invalid property '%s': %s" % (property, msg))


###################################################################
# Still to port.
# Original lines are prefixed with "#   "
#
#   
#   import utility : ungrist ;
#   import sequence : unique ;
#   import errors : error ;
#   import feature ;
#   import regex ;
#   import sequence ;
#   import set ;
#   import path ;
#   import assert ;
#   
#   


#   rule validate-property-sets ( property-sets * )
#   {
#       for local s in $(property-sets)
#       {
#           validate [ feature.split $(s) ] ;
#       }
#   }
#

def remove(attributes, properties):
    """Returns a property sets which include all the elements
    in 'properties' that do not have attributes listed in 'attributes'."""
    
    result = []
    for e in properties:
        attributes_new = feature.attributes(get_grist(e))
        has_common_features = 0
        for a in attributes_new:
            if a in attributes:
                has_common_features = 1
                break

        if not has_common_features:
            result += e

    return result


def take(attributes, properties):
    """Returns a property set which include all
    properties in 'properties' that have any of 'attributes'."""
    result = []
    for e in properties:
        if set.intersection(attributes, feature.attributes(get_grist(e))):
            result.append(e)
    return result


class PropertyMap:
    """ Class which maintains a property set -> string mapping.
    """
    def __init__ (self):
        self.__properties = []
        self.__values = []
    
    def insert (self, properties, value):
        """ Associate value with properties.
        """
        self.__properties.append(properties)
        self.__values.append(value)

    def find (self, properties):
        """ Return the value associated with properties
        or any subset of it. If more than one
        subset has value assigned to it, return the
        value for the longest subset, if it's unique.
        """
        return self.find_replace (properties)

    def find_replace(self, properties, value=None):
        matches = []
        match_ranks = []
        
        for i in range(0, len(self.__properties)):
            p = self.__properties[i]
                        
            if set.contains (p, properties):
                matches.append (i)
                match_ranks.append(len(p))

        best = sequence.select_highest_ranked (matches, match_ranks)

        if not best:
            return None

        if len (best) > 1:
            raise NoBestMatchingAlternative ()

        best = best [0]
            
        original = self.__values[best]

        if value:
            self.__values[best] = value

        return original

#   local rule __test__ ( )
#   {
#       import errors : try catch ;
#       import feature ;
#       import feature : feature subfeature compose ;
#       
#       # local rules must be explicitly re-imported
#       import property : path-order ;
#       
#       feature.prepare-test property-test-temp ;
#   
#       feature toolset : gcc : implicit symmetric ;
#       subfeature toolset gcc : version : 2.95.2 2.95.3 2.95.4
#         3.0 3.0.1 3.0.2 : optional ;
#       feature define : : free ;
#       feature runtime-link : dynamic static : symmetric link-incompatible ;
#       feature optimization : on off ;
#       feature variant : debug release : implicit composite symmetric ;
#       feature rtti : on off : link-incompatible ;
#   
#       compose <variant>debug : <define>_DEBUG <optimization>off ;
#       compose <variant>release : <define>NDEBUG <optimization>on ;
#   
#       import assert ;
#       import "class" : new ;
#       
#       validate <toolset>gcc  <toolset>gcc-3.0.1 : $(test-space) ;
#       
#       assert.result <toolset>gcc <rtti>off <define>FOO
#           : refine <toolset>gcc <rtti>off
#           : <define>FOO
#           : $(test-space)
#           ;
#   
#       assert.result <toolset>gcc <optimization>on
#           : refine <toolset>gcc <optimization>off
#           : <optimization>on
#           : $(test-space)
#           ;
#   
#       assert.result <toolset>gcc <rtti>off
#           : refine <toolset>gcc : <rtti>off : $(test-space)
#           ;
#   
#       assert.result <toolset>gcc <rtti>off <rtti>off:<define>FOO
#           : refine <toolset>gcc : <rtti>off <rtti>off:<define>FOO 
#           : $(test-space)
#           ;
#       
#       assert.result <toolset>gcc:<define>foo <toolset>gcc:<define>bar 
#           : refine <toolset>gcc:<define>foo : <toolset>gcc:<define>bar 
#           : $(test-space)
#           ;
#   
#       assert.result <define>MY_RELEASE
#           : evaluate-conditionals-in-context 
#             <variant>release,<rtti>off:<define>MY_RELEASE
#             : <toolset>gcc <variant>release <rtti>off
#                    
#           ;
#   
#       try ;
#           validate <feature>value : $(test-space) ;
#       catch "Invalid property '<feature>value': unknown feature 'feature'." ;
#   
#       try ;
#           validate <rtti>default : $(test-space) ;
#       catch \"default\" is not a known value of feature <rtti> ;
#       
#       validate <define>WHATEVER : $(test-space) ;
#   
#       try ;
#           validate <rtti> : $(test-space) ;
#       catch "Invalid property '<rtti>': No value specified for feature 'rtti'." ;
#   
#       try ;
#           validate value : $(test-space) ;
#       catch "value" is not a value of an implicit feature ;
#              
#   
#       assert.result <rtti>on 
#           : remove free implicit : <toolset>gcc <define>foo <rtti>on : $(test-space) ;
#   
#       assert.result <include>a 
#           : select include : <include>a <toolset>gcc ;
#   
#       assert.result <include>a 
#           : select include bar : <include>a <toolset>gcc ;
#   
#       assert.result <include>a <toolset>gcc
#           : select include <bar> <toolset> : <include>a <toolset>gcc ;
#       
#       assert.result <toolset>kylix <include>a 
#           : change <toolset>gcc <include>a : <toolset> kylix ;
#   
#       # Test ordinary properties 
#       assert.result 
#         : split-conditional <toolset>gcc 
#         ;
#       
#       # Test properties with ":"
#       assert.result
#         : split-conditional <define>FOO=A::B
#         ;
#       
#       # Test conditional feature
#       assert.result <toolset>gcc,<toolset-gcc:version>3.0 <define>FOO
#         : split-conditional <toolset>gcc,<toolset-gcc:version>3.0:<define>FOO
#         ;
#       
#       feature.finish-test property-test-temp ;
#   }
#   
    
