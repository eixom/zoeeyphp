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

#include "php.h"
#include "php_zoeey.h"
#include "status.h"

#define ZE_LABEL     "label"
#define ZE_NAME      "name"
#define ZE_BRIEF     "brief"

/** {{{ current ZeStatus object
 */
zend_class_entry * ze_status_ce;
/* }}} */

/** {{{ public ZeStatus::__construct()
 */
PHP_METHOD(ze_status, __construct) {
    zval * self           = NULL;
    zval * label          = NULL;
    zval * name           = NULL;
    zval * brief          = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zz"
                             , &label
                             , &name
                             , &brief
                             ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    zend_update_property(ze_status_ce, self, ZEND_STRL(ZE_LABEL) , label TSRMLS_CC);

    if (name) {
        zend_update_property(ze_status_ce, self, ZEND_STRL(ZE_NAME)  , name  TSRMLS_CC);
    }

    if (brief) {
        zend_update_property(ze_status_ce, self, ZEND_STRL(ZE_BRIEF) , brief TSRMLS_CC);
    }

}
/* }}} */

/** {{{ public ZeStatus::getLabel()
 */
PHP_METHOD(ze_status, getLabel) {
    zval *   self         = NULL;
    zval *   label        = NULL;

    self = getThis();

    label = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_LABEL), 0 TSRMLS_CC);
    
    RETURN_ZVAL(label, 1, 0);
}
/* }}} */

/** {{{ public ZeStatus::getName()
 */
PHP_METHOD(ze_status, getName) {
    zval * self        = NULL;
    zval * name        = NULL;

    self = getThis();

    name = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_NAME), 1 TSRMLS_CC);
    
    if (name) {
        RETURN_ZVAL(name, 1, 0);
    }

    RETURN_NULL();

}
/* }}} */

/** {{{ public ZeStatus::getBrief()
 */
PHP_METHOD(ze_status, getBrief) {
    zval *  self        = NULL;
    zval *  brief       = NULL;
    zval *  name_z      = NULL;
    char *  name        = NULL;
    int     name_len    = 0;
    zval ** msg_pp      = NULL;
    zval *  msg_p       = NULL;
    zval *  msg            = NULL;
    zval    is_debug          ;
    char *  debug_msg   = NULL;
    int     len         = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s"
                            , &name, &name_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    brief = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_BRIEF), 1 TSRMLS_CC);
    
    if (!brief) {
        RETURN_NULL();
    } 
    
    if (Z_TYPE_P(brief) == IS_ARRAY && name_len) {
    
        if (zend_hash_find(Z_ARRVAL_P(brief), name, name_len + 1, (void **) &msg_pp) == SUCCESS) {
             
            ALLOC_INIT_ZVAL(msg);
            *msg = **msg_pp;
        }
    } else if (Z_TYPE_P(brief) == IS_OBJECT && name_len) {
    
            msg_p = zend_read_property(Z_OBJCE_P(brief), brief, name, name_len , 1 TSRMLS_CC);
            ALLOC_INIT_ZVAL(msg);
            *msg = *msg_p;
    } else if (Z_TYPE_P(brief) == IS_STRING) {

            ALLOC_INIT_ZVAL(msg);
            *msg = *brief;
    } 

    if (!msg) {
        RETURN_NULL();
    }
    
    zval_copy_ctor(msg);
    INIT_PZVAL(msg);
    
    if (zend_get_constant(ZEND_STRL(ZE_DEBUG), & is_debug TSRMLS_CC)) {

        name_z = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_NAME), 0 TSRMLS_CC);

        if (Z_BVAL(is_debug) && Z_TYPE_P(name_z) == IS_STRING) {
            len = spprintf(&debug_msg, len, "[%s]%s", Z_STRVAL_P(name_z), Z_STRVAL_P(msg));
            ZE_RE_DEFINE(msg);
            ZVAL_STRINGL(msg, debug_msg, len, 1);
            efree(debug_msg);
        }
        ZE_DTOR(is_debug);
    }
    
    RETURN_ZVAL(msg, 1, 1);
}
/* }}} */

/** {{{ public ZeStatus::toArray()
 */
PHP_METHOD(ze_status, toArray) {
    zval *   self        = NULL;
    zval *   array       = NULL;
    zval *   label       = NULL;
    zval *   name        = NULL;
    zval *   brief       = NULL;

    self = getThis();

    label = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_LABEL), 0 TSRMLS_CC);
    name  = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_NAME) , 1 TSRMLS_CC);
    brief = zend_read_property(ze_status_ce, self, ZEND_STRL(ZE_BRIEF), 1 TSRMLS_CC);

    ZE_NEW_ARRAY(array);
    Z_ADDREF_P(label);
    add_assoc_zval(array, "label", label);

    if (name){

       Z_ADDREF_P(name);
       add_assoc_zval(array, "name", name);
    } else {

       add_assoc_null(array, "name");
    }
    if (brief){

       Z_ADDREF_P(brief);
       add_assoc_zval(array, "brief", brief);
    } else {

       add_assoc_null(array, "brief");
    }
    
    RETURN_ZVAL(array, 1, 1);

}
/* }}} */

/** {{{ conn
 */
ZEND_BEGIN_ARG_INFO(status_construct_args, 0)
    ZEND_ARG_INFO(0, label)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, brief)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ conn
 */
ZEND_BEGIN_ARG_INFO(status_getBrief_args, 0)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{
 */
zend_function_entry ze_status_methods[] = {
    PHP_ME(ze_status, __construct     , status_construct_args  , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(ze_status, getLabel        , NULL                   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_status, getName         , NULL                   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_status, getBrief        , status_getBrief_args   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_status, toArray         , NULL                   , ZEND_ACC_PUBLIC) {
        NULL, NULL, NULL
    }
};
/* }}} */

/** {{{
 */
PHP_MINIT_FUNCTION(ze_status) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "ZeStatus", ze_status_methods);
    ze_status_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

    zend_declare_property_null(ze_status_ce, ZEND_STRL(ZE_LABEL) , ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(ze_status_ce, ZEND_STRL(ZE_NAME)  , ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(ze_status_ce, ZEND_STRL(ZE_BRIEF) , ZEND_ACC_PRIVATE TSRMLS_CC);

}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
