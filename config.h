/**
 * @Author: YangGuang <sunshine>
 * @Date:   2018-01-27T14:53:33+08:00
 * @Email:  guang334419520@126.com
 * @Filename: stl_simple_config.h
 * @Last modified by:   sunshine
 * @Last modified time: 2018-02-27T16:12:42+08:00
 */
#ifndef GSTL_CONFIG_H
#define GSTL_CONFIG_H







 # ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
 #   define __STL_NULL_TMPL_ARGS <>
 # else
 #   define __STL_NULL_TMPL_ARGS
 # endif

 #define __STL_TEMPLATE_NULL template<>


# define __GSTL gstl
# define __GSTL_BEGIN_NAMESPACE namespace gstl {
# define __GSTL_END_NAMESPACE }







# endif/* __STL_CONFIG_H */

// Local Variables:
// mode:C++
// End:
