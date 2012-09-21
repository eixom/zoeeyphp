/*
   +----------------------------------------------------------------------+
   | ZoeeyPHP Framework                                                   |
   +----------------------------------------------------------------------+
   | Copyright (c) 2011 moxie(system128@gmail.com)                        |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
 */

#ifndef ZE_HELPER_H
#define ZE_HELPER_H
PHP_ZOEEY_API void ze_error(int type TSRMLS_DC, const char * name, ...);
PHP_ZOEEY_API zend_bool ze_str_is_endwith(const char * str, int str_len
									 , const char * end, int end_len);
PHP_ZOEEY_API zval * ze_names_to_list(char * names,int names_len);

PHP_ZOEEY_API zval * ze_call_php_function(char *function_name, int function_name_len
										, zval **retval_ptr_ptr
										, int param_count, zval *** params TSRMLS_DC);

PHP_ZOEEY_API zval * ze_call_method(zval ** object_pp, zend_class_entry * obj_ce
								  , zend_function ** fn_proxy
								  , char * function_name 
								  , int function_name_len
								  , zval ** retval_ptr_ptr
								  , int param_count
								  , zval *** params TSRMLS_DC);


#endif    /* ZE_HELPER_H */

