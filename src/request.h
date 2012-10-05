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

#ifndef ZE_LOADER_H
#define ZE_LOADER_H

#define ZE_TRACK_VARS_SESSION 10000

extern zend_class_entry * ze_loader_ce;

zval * ze_loader_instance(TSRMLS_D);

PHP_ZOEEY_API zval ** get_global_by_name(zval * values, char * name, int name_len, char * key, int key_len, zval * def TSRMLS_DC);
PHP_ZOEEY_API zval ** get_track(zval * values, char * key, int key_len, zval * def, int track TSRMLS_DC);
PHP_ZOEEY_API zval  * get_ip(zend_bool is_format TSRMLS_DC);
PHP_ZOEEY_API void    from_ip(zval * fields, zval * names, zend_bool is_format TSRMLS_DC);
PHP_ZOEEY_API void    from_track_var(zval * values,zval * fields, zval * names, zval * def, int track TSRMLS_DC);


PHP_MINIT_FUNCTION(ze_loader);

#endif    /* ZE_LOADER_H */

