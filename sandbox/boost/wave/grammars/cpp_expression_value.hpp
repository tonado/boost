/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    http://spirit.sourceforge.net/

    Copyright (c) 2001-2004 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(CPP_EXPRESSION_VALUE_HPP_452FE66D_8754_4107_AF1E_E42255A0C18A_INCLUDED)
#define CPP_EXPRESSION_VALUE_HPP_452FE66D_8754_4107_AF1E_E42255A0C18A_INCLUDED

#if defined (BOOST_SPIRIT_DEBUG)
#include <iostream>
#endif // defined(BOOST_SPIRIT_DEBUG)

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace grammars {
namespace closures {

///////////////////////////////////////////////////////////////////////////////
//
//  The closure_value class represents the closure type, which is used for the 
//  expression grammar. 
//
//      This class was introduced to allow the expression grammar to respect 
//      the numeric type of a numeric literal or expression result.
//
///////////////////////////////////////////////////////////////////////////////
class closure_value {

    typedef unsigned long ulong_t;

public:

    enum value_type {
        is_int = 1,
        is_uint = 2,
        is_bool = 3
    };
    
    closure_value(bool valid_ = true) 
    : type(is_int), valid(valid_) 
    { value.i = 0; }
    explicit closure_value(int i, bool valid_ = true) 
    : type(is_int), valid(valid_) 
    { value.i = i; }
    explicit closure_value(unsigned int ui, bool valid_ = true) 
    : type(is_uint), valid(valid_) 
    { value.ui = ui; }
    explicit closure_value(long i, bool valid_ = true) 
    : type(is_int), valid(valid_) 
    { value.i = i; }
    explicit closure_value(ulong_t ui, bool valid_ = true) 
    : type(is_uint), valid(valid_) 
    { value.ui = ui; }
    explicit closure_value(bool b, bool valid_ = true) 
    : type(is_bool), valid(valid_) 
    { value.b = b; }

    value_type get_type() const { return type; }
    bool is_valid() const { return valid; }
    
// implicit conversion
    operator int() const 
    {
        switch (type) {
        case is_uint:   return value.ui;
        case is_bool:   return value.b ? 1 : 0;
        case is_int:    break;
        }
        return value.i;
    }
    operator unsigned int() const 
    {
        switch (type) {
        case is_uint:   return value.ui;
        case is_bool:   return value.b ? 1 : 0;
        case is_int:    break;
        }
        return value.i;
    }
    operator long() const 
    {
        switch (type) {
        case is_uint:   return value.ui;
        case is_bool:   return value.b ? 1 : 0;
        case is_int:    break;
        }
        return value.i;
    }
    operator ulong_t() const 
    {
        switch (type) {
        case is_uint:   return value.ui;
        case is_bool:   return value.b ? 1 : 0;
        case is_int:    break;
        }
        return value.i;
    }
    operator bool() const 
    {
        switch (type) {
        case is_uint:   return value.ui != 0;
        case is_bool:   return value.b;
        case is_int:    break;
        }
        return value.i != 0.0;
    }

// assignment    
    closure_value &operator= (closure_value const &rhs)
    {
        switch (rhs.get_type()) {
        case is_int:    
            value.i = long(rhs); 
            type = is_int;
            break;
        
        case is_uint:   
            value.ui = ulong_t(rhs); 
            type = is_uint;
            break;
        
        case is_bool:   
            value.b = bool(rhs);
            type = is_bool;
            break;
        }
        valid = rhs.valid;
        return *this;
    }
    closure_value &operator= (int rhs)
    {
        type = is_int;
        value.i = rhs;
        valid = true;
        return *this;
    }
    closure_value &operator= (unsigned int rhs)
    {
        type = is_uint;
        value.ui = rhs;
        valid = true;
        return *this;
    }
    closure_value &operator= (long rhs)
    {
        type = is_int;
        value.i = rhs;
        valid = true;
        return *this;
    }
    closure_value &operator= (ulong_t rhs)
    {
        type = is_uint;
        value.ui = rhs;
        valid = true;
        return *this;
    }
    closure_value &operator= (bool rhs)
    {
        type = is_bool;
        value.b = rhs;
        valid = true;
        return *this;
    }

// arithmetics
    closure_value &operator+= (closure_value const &rhs)
    {
        switch (type) {
        case is_int:    
            switch(rhs.type) {
            case is_bool:   value.i += long(rhs); break;
            case is_int:    value.i += rhs.value.i; break;
            case is_uint:   value.ui += rhs.value.ui; type = is_uint; break;
            }
            break;
            
        case is_uint:   value.ui += ulong_t(rhs); break;
        case is_bool:   
            value.i = value.b + bool(rhs);
            type = is_int;
        }
        valid = valid && rhs.valid;
        return *this;
    }
    closure_value &operator-= (closure_value const &rhs)
    {
        switch (type) {
        case is_int:
            switch(rhs.type) {
            case is_bool:   value.i -= long(rhs); break;
            case is_int:    value.i -= rhs.value.i; break;
            case is_uint:   value.ui -= rhs.value.ui; type = is_uint; break;
            }
            break;
            
        case is_uint:   value.ui -= ulong_t(rhs); break;
        case is_bool:   
            value.i = value.b - bool(rhs);
            type = is_int;
        }
        valid = valid && rhs.valid;
        return *this;
    }
    closure_value &operator*= (closure_value const &rhs)
    {
        switch (type) {
        case is_int:    
            switch(rhs.type) {
            case is_bool:   value.i *= long(rhs); break;
            case is_int:    value.i *= rhs.value.i; break;
            case is_uint:   value.ui *= rhs.value.ui; type = is_uint; break;
            }
            break;
            
        case is_uint:   value.ui *= ulong_t(rhs); break;
        case is_bool:
            switch (rhs.type) {
            case is_int:
                value.i = (value.b ? 1 : 0) * rhs.value.i; 
                type = is_int; 
                break;
                
            case is_uint:
                value.ui = (value.b ? 1 : 0) * rhs.value.ui; 
                type = is_uint; 
                break;
                
            case is_bool:
                value.b = 0 != ((value.b ? 1 : 0) * (rhs.value.b ? 1 : 0));
                break;
            }
        }
        valid = valid && rhs.valid;
        return *this;
    }
    closure_value &operator/= (closure_value const &rhs)
    {
        switch (type) {
        case is_int:    
            switch(rhs.type) {
            case is_bool:   
            case is_int:
                if (valid && long(rhs) != 0)
                    value.i /= long(rhs); 
                else
                    valid = false;      // division by zero
                break;
                
            case is_uint:
                if (valid && rhs.value.ui != 0) {
                    value.ui /= rhs.value.ui; 
                    type = is_uint; 
                }
                else {
                    valid = false;      // division by zero
                }
                break;
            }
            break;
            
        case is_uint: 
            if (valid && ulong_t(rhs) != 0) 
                value.ui /= ulong_t(rhs); 
            else
                valid = false;      // division by zero
            break;

        case is_bool:  
            if (valid && bool(rhs)) {
                switch(rhs.type) {
                case is_int:
                    value.i = (value.b ? 1 : 0) / rhs.value.i;
                    type = is_int;
                    break;
                    
                case is_uint:
                    value.i = (value.b ? 1 : 0) / rhs.value.ui;
                    type = is_int;
                    break;
                    
                case is_bool:
                    break;
                }                    
            }
            else {
                valid = false;      // division by zero
            }
        }
        return *this;
    }

    friend closure_value 
    operator- (closure_value const &rhs)
    {
        switch (rhs.type) {
        case is_int:    return closure_value(-long(rhs), rhs.valid);
        case is_bool:   return closure_value(!bool(rhs), rhs.valid); 
        case is_uint:   break;
        }
        return closure_value(-(int)ulong_t(rhs), rhs.valid);
    }
    friend closure_value 
    operator! (closure_value const &rhs)
    {
        switch (rhs.type) {
        case is_int:    return closure_value(!long(rhs), rhs.valid);
        case is_bool:   return closure_value(!bool(rhs), rhs.valid); 
        case is_uint:   break;
        }
        return closure_value(!ulong_t(rhs), rhs.valid);
    }
    
// comparison
    friend closure_value 
    operator== (closure_value const &lhs, closure_value const &rhs)
    {
        bool cmp = false;
        switch (lhs.type) {
        case is_int:
            switch(rhs.type) {
            case is_bool:   cmp = bool(lhs) == rhs.value.b; break;
            case is_int:    cmp = lhs.value.i == rhs.value.i; break;
            case is_uint:   cmp = lhs.value.ui == rhs.value.ui; break;
            }
            break;
            
        case is_uint:   cmp = lhs.value.ui == ulong_t(rhs); break;
        case is_bool:   cmp = lhs.value.b == bool(rhs); break;
        }
        return closure_value(cmp, lhs.valid && rhs.valid);
    }
    friend closure_value 
    operator!= (closure_value const &lhs, closure_value const &rhs)
    {
        return closure_value(!bool(lhs == rhs), lhs.valid && rhs.valid);
    }
    friend closure_value 
    operator> (closure_value const &lhs, closure_value const &rhs)
    {
        bool cmp = false;
        switch (lhs.type) {
        case is_int:
            switch(rhs.type) {
            case is_bool:   cmp = lhs.value.i > long(rhs); break;
            case is_int:    cmp = lhs.value.i > rhs.value.i; break;
            case is_uint:   cmp = lhs.value.ui > rhs.value.ui; break;
            }
            break;
            
        case is_uint:   cmp = lhs.value.ui > ulong_t(rhs); break;
        case is_bool:   cmp = lhs.value.b > bool(rhs); break;
        }
        return closure_value(cmp, lhs.valid && rhs.valid);
    }
    friend closure_value 
    operator< (closure_value const &lhs, closure_value const &rhs)
    {
        bool cmp = false;
        switch (lhs.type) {
        case is_int:    cmp = long(lhs) < long(rhs); break;
            switch(rhs.type) {
            case is_bool:   cmp = lhs.value.i < long(rhs); break;
            case is_int:    cmp = lhs.value.i < rhs.value.i; break;
            case is_uint:   cmp = lhs.value.ui < rhs.value.ui; break;
            }
            break;
            
        case is_uint:   cmp = lhs.value.ui < ulong_t(rhs); break;
        case is_bool:   cmp = bool(lhs) < bool(rhs); break;
        }
        return closure_value(cmp, lhs.valid && rhs.valid);
    }
    friend closure_value 
    operator<= (closure_value const &lhs, closure_value const &rhs)
    {
        return closure_value(!bool(lhs > rhs), lhs.valid && rhs.valid);
    }
    friend closure_value 
    operator>= (closure_value const &lhs, closure_value const &rhs)
    {
        return closure_value(!bool(lhs < rhs), lhs.valid && rhs.valid);
    }

    closure_value &
    operator<<= (closure_value const &rhs)
    {
        switch (type) {
        case is_bool:
        case is_int:
            switch (rhs.type) {
            case is_bool:
            case is_int:
                {
                long shift_by = long(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    else if (shift_by < -64)
                        shift_by = -64;
                    value.i <<= shift_by; 
                }
                break;
                
            case is_uint:
                {
                unsigned long shift_by = ulong_t(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    value.ui <<= shift_by; 
                
                // Note: The usual arithmetic conversions are not performed on 
                //       bit shift operations.
                }
                break;
            }
            break;

        case is_uint:
            switch (rhs.type) {
            case is_bool:
            case is_int:
                {
                long shift_by = long(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    else if (shift_by < -64)
                        shift_by = -64;
                    value.ui <<= shift_by; 
                }
                break;
                
            case is_uint:
                {
                unsigned long shift_by = ulong_t(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    value.ui <<= shift_by; 
                }
                break;
            }
        }
        valid = valid && rhs.valid;
        return *this;
    }

    closure_value &
    operator>>= (closure_value const &rhs)
    {
        switch (type) {
        case is_bool:
        case is_int:
            switch (rhs.type) {
            case is_bool:
            case is_int:
                {
                long shift_by = long(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    else if (shift_by < -64)
                        shift_by = -64;
                    value.i >>= shift_by; 
                }
                break;
                
            case is_uint:
                {
                unsigned long shift_by = ulong_t(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    value.ui >>= shift_by; 
                
                // Note: The usual arithmetic conversions are not performed on 
                //       bit shift operations.
                }
                break;
            }
            break;
            
        case is_uint:
            switch (rhs.type) {
            case is_bool:
            case is_int:
                {
                long shift_by = long(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    else if (shift_by < -64)
                        shift_by = -64;
                    value.ui >>= shift_by; 
                }
                break;
                
            case is_uint:
                {
                unsigned long shift_by = ulong_t(rhs);
                    
                    if (shift_by > 64) 
                        shift_by = 64;
                    value.ui >>= shift_by; 
                }
                break;
            }
            break;
        }
        valid = valid && rhs.valid;
        return *this;
    }

    friend closure_value 
    operator|| (closure_value const &lhs, closure_value const &rhs)
    {
        bool result = bool(lhs) || bool(rhs);
        return closure_value(result, lhs.valid && rhs.valid);
    }
    
    friend closure_value 
    operator&& (closure_value const &lhs, closure_value const &rhs)
    {
        bool result = bool(lhs) && bool(rhs);
        return closure_value(result, lhs.valid && rhs.valid);
    }

    // handle the ?: operator
    closure_value &
    handle_questionmark(closure_value const &cond, closure_value const &val2)
    {
        switch (type) {
        case is_int:
            switch (val2.type) {
            case is_bool: value.b = bool(cond) ? value.b : bool(val2); break;
            case is_int:  value.i = bool(cond) ? value.i : long(val2); break;
            case is_uint: 
                value.ui = bool(cond) ? value.ui : ulong_t(val2); 
                type = is_uint;   // changing type!
                break;
            }
            break;
            
        case is_uint:   value.ui = bool(cond) ? value.ui : ulong_t(val2); break;
        case is_bool:   value.b = bool(cond) ? value.b : bool(val2); break;
        }
        valid = bool(cond) ? valid : val2.valid;
        return *this;
    }
    
#if defined (BOOST_SPIRIT_DEBUG)
    friend std::ostream&
    operator<< (std::ostream &o, closure_value const &val)
    {
        switch (val.type) {
        case is_int:    o << "int(" << long(val) << ")"; break;
        case is_uint:   o << "unsigned int(" << ulong_t(val) << ")"; break;
        case is_bool:   o << "bool(" << bool(val) << ")"; break;
        }
        return o;
    }
#endif // defined(BOOST_SPIRIT_DEBUG)

private:
    value_type type;
    union {
        long i;
        ulong_t ui;
        bool b;
    } value;
    bool valid;
};

///////////////////////////////////////////////////////////////////////////////
}   // namespace closures
}   // namespace grammars
}   // namespace wave
}   // namespace boost

#endif // !defined(CPP_EXPRESSION_VALUE_HPP_452FE66D_8754_4107_AF1E_E42255A0C18A_INCLUDED)
