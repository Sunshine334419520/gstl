/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-27T14:53:33+08:00
 * @Email:  guang334419520@126.com
 * @Filename: stl_simple_config.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-01-31T20:19:04+08:00
 */
#ifndef __STL_SIMPLE_CONFIG_H
#define __STL_SIMPLE_CONFIG_H







 # ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
 #   define __STL_NULL_TMPL_ARGS <>
 # else
 #   define __STL_NULL_TMPL_ARGS
 # endif

 # ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
 #   define __STL_TEMPLATE_NULL template<>
 # else
 #   define __STL_TEMPLATE_NULL
 # endif


# define __STD_SIMPLE simple_stl
# define __STL_SIMPLE_BEGIN_NAMESPACE namespace simple_stl {
# define __STL_SIMPLE_END_NAMESPACE }







# endif/* __STL_CONFIG_H */

// Local Variables:
// mode:C++
// End:
