#include "actions.h"
#include "driver/output.h"

#include <iostream>

void
print (const context_t::iterator_type  & start, 
       const context_t::iterator_type  & end) {
	context_t::iterator_type s = start;
	std::cout << "(";
	while (s != end) {
		std::cout << s->get_value ();
		++s;
	}
	std::cout << ")" << std::endl;
}

void
break_here (context_iter_t ,context_iter_t ) {
	std::cout << "internal break" << std::endl;
}

ref_value_actor<TransformContext,register_actor<MethodBodyXForm> >
method_body( TransformContext * ctx ) {
	return ref_value_actor<TransformContext,
	                       register_actor<MethodBodyXForm> >(*ctx);
}

ref_value_actor<TransformContext,register_actor<ExportStmtXForm> >
export_stmt ( TransformContext * ctx ) {
	return ref_value_actor<TransformContext,
	                       register_actor<ExportStmtXForm> >(*ctx);
}

ref_value_actor<TransformContext,register_actor<AccessCtlXForm> >
access_spec ( TransformContext * ctx ) {
// 	cerr << "[access_spec()]";
	return ref_value_actor<TransformContext,
	                       register_actor<AccessCtlXForm> >(*ctx);

}

ref_value_actor<TransformContext,register_actor<AccessCtlXForm> >
access_default ( TransformContext * ctx ) {
// 	cerr << "[access_default()]";
	return ref_value_actor<TransformContext,
	                       register_actor<AccessCtlXForm> >(*ctx);
}

ref_value_actor<TransformContext,register_actor<ImportStmtXForm> >
import_stmt ( TransformContext * ctx ) {
	return ref_value_actor<TransformContext,
	                       register_actor<ImportStmtXForm> >(*ctx);
}

