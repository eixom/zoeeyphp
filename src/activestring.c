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
#include "ext/standard/php_smart_str.h"
#include "ext/standard/php_array.h"

#include "helper.h"
#include "activestring.h"


#define ZE_ITEMS   "items"
#define ZE_ACTIVES "actives"

/** {{{ current activestring object
 */
zend_class_entry * ze_activestring_ce;
/* }}} */


/** {{{ public ZeActiveString::__construct()
 */
PHP_METHOD(ze_activestring, __construct) {
    zval * self    = NULL;
    zval * items   = NULL;
    zval * actives = NULL;

    self = getThis();

    ZE_NEW_ARRAY(items);
    ZE_NEW_ARRAY(actives);

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS)    , items TSRMLS_CC);
    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES)  , actives TSRMLS_CC);

    ZE_PTR_DTOR(items);
    ZE_PTR_DTOR(actives);


}
/* }}} */


/** {{{ public ZeActiveString::putAll($map)
 */
PHP_METHOD(ze_activestring, putAll) {
    zval * self       = NULL;
    zval * items      = NULL;
    zval * map        = NULL;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a"
                            , &map
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    items = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS), 0 TSRMLS_CC);

    php_array_merge(Z_ARRVAL_P(items), Z_ARRVAL_P(map), 0 TSRMLS_CC);

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS), items TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */


/** {{{ public ZeActiveString::put($condition, $val)
 */
PHP_METHOD(ze_activestring, put) {
    zval * self       = NULL;
    zval * items      = NULL;
    char * cond       = NULL;
    int    cond_len   = 0;
    char * val        = NULL;
    int    val_len    = 0;



    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss"
                            , &cond, &cond_len
                            , &val , &val_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    items = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS), 0 TSRMLS_CC);

    add_assoc_stringl(items, cond, val, val_len, 1);


    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS), items TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */


/** {{{ public ZeActiveString::active($condition)
 */
PHP_METHOD(ze_activestring, active) {
    zval * self       = NULL;
    zval * actives    = NULL;
    char * cond       = NULL;
    int    cond_len   = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
                            , &cond, &cond_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    actives = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), 0 TSRMLS_CC);

    add_assoc_bool(actives, cond, 1);

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), actives TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */


/** {{{ public ZeActiveString::cancel($condition)
 */
PHP_METHOD(ze_activestring, cancel) {
    zval * self       = NULL;
    zval * actives    = NULL;
    char * cond       = NULL;
    int    cond_len   = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
                            , &cond, &cond_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    actives = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), 0 TSRMLS_CC);

    zend_hash_del(Z_ARRVAL_P(actives), cond, cond_len + 1);

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), actives TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeActiveString::update($oldCondition, $newCondition)
 */
PHP_METHOD(ze_activestring, update) {
    zval * self           = NULL;
    zval * actives        = NULL;
    char * old_cond       = NULL;
    int    old_cond_len   = 0;
    char * new_cond       = NULL;
    int    new_cond_len   = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss"
                            , &old_cond, &old_cond_len
                            , &new_cond, &new_cond_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self = getThis();

    actives = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), 0 TSRMLS_CC);

    zend_hash_del(Z_ARRVAL_P(actives), old_cond, old_cond_len + 1);

    add_assoc_bool(actives, new_cond, 1);

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), actives TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */


/** {{{ public ZeActiveString::clear()
 */
PHP_METHOD(ze_activestring, clear) {
    zval * self           = NULL;
    zval * actives        = NULL;

    self = getThis();

    actives = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), 0 TSRMLS_CC);

    zend_hash_clean(Z_ARRVAL_P(actives));

    zend_update_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), actives TSRMLS_CC);

    RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeActiveString::toString($separator)
 */
PHP_METHOD(ze_activestring, toString) {
    zval * self        = NULL;
    zval * items       = NULL;
    zval * actives     = NULL;
    char * sep         = NULL;
    int    sep_len     = 0;
    smart_str sstr     = {NULL, 0, 0};

    HashTable * items_table     = NULL;
    HashPosition items_pointer  = NULL;
    HashTable * actives_table   = NULL;
    char  * key                 = NULL;
    uint    key_len             = 0;
    ulong   idx                 = 0;
    zval ** val                 = NULL;
    char  * str                 = NULL;
    int     str_len             = 0;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s"
                            , &sep, &sep_len
                            ) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    self    = getThis();

    items   = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ITEMS)  , 0 TSRMLS_CC);

    actives = zend_read_property(ze_activestring_ce, self, ZEND_STRL(ZE_ACTIVES), 0 TSRMLS_CC);

    do {
        if (!items || Z_TYPE_P(items) != IS_ARRAY) {
            ZE_PTR_DTOR(items);
            ze_error(E_ERROR TSRMLS_CC, "recorder.toString.items.not_array");
            break;
        }

        if (!actives || Z_TYPE_P(actives) != IS_ARRAY) {
            ZE_PTR_DTOR(actives);
            ze_error(E_ERROR TSRMLS_CC, "recorder.toString.actives.not_array");
            break;
        }

        items_table   = Z_ARRVAL_P(items);

        actives_table = Z_ARRVAL_P(actives);

        for (zend_hash_internal_pointer_reset_ex(items_table, &items_pointer);
                  zend_hash_get_current_key_ex(items_table, &key, &key_len, &idx, 0, &items_pointer) == HASH_KEY_IS_STRING;
                  zend_hash_move_forward_ex(items_table, &items_pointer)) {
              if (!key || !key_len) {
                  continue;
              }

              if (zend_hash_exists(actives_table, key, key_len )) {
                if (zend_hash_get_current_data_ex(items_table
                                            , (void**) &val, &items_pointer) == SUCCESS) {


                    smart_str_appendl(&sstr, Z_STRVAL_PP(val), Z_STRLEN_PP(val));
                    if (sep_len) {
                      smart_str_appendl(&sstr, sep, sep_len);
                    }

                }
              }
        }
        smart_str_0(&sstr);
    } while(0);

    if (sstr.len) {
      str_len = sstr.len - sep_len;
      spprintf(&str, str_len, "%s", sstr.c);
      smart_str_free(&sstr);
      RETURN_STRING(str , 0);
    }else{
      smart_str_free(&sstr);
      RETURN_EMPTY_STRING();
    }

}
/* }}} */

/** {{{ $map
 */
ZEND_BEGIN_ARG_INFO(activestring_putAll_args, 0)
    ZEND_ARG_INFO(0, map)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $condition, $val
 */
ZEND_BEGIN_ARG_INFO(activestring_put_args, 0)
    ZEND_ARG_INFO(0, condition)
    ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $condition
 */
ZEND_BEGIN_ARG_INFO(activestring_active_args, 0)
    ZEND_ARG_INFO(0, condition)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $condition
 */
ZEND_BEGIN_ARG_INFO(activestring_cancel_args, 0)
    ZEND_ARG_INFO(0, condition)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $oldCondition, $newCondition
 */
ZEND_BEGIN_ARG_INFO(activestring_update_args, 0)
    ZEND_ARG_INFO(0, oldCondition)
    ZEND_ARG_INFO(0, newCondition)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $sep = ''
 */
ZEND_BEGIN_ARG_INFO(activestring_toString_args, 0)
    ZEND_ARG_INFO(0, separator)
ZEND_END_ARG_INFO()
/* }}} */


/** {{{
 */
zend_function_entry ze_activestring_methods[] = {
    PHP_ME(ze_activestring, __construct , NULL, ZEND_ACC_PUBLIC  | ZEND_ACC_CTOR)
    PHP_ME(ze_activestring, putAll      , activestring_putAll_args   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, put         , activestring_put_args      , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, active      , activestring_active_args   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, cancel      , activestring_cancel_args   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, update      , activestring_update_args   , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, clear       , NULL                       , ZEND_ACC_PUBLIC)
    PHP_ME(ze_activestring, toString    , activestring_toString_args , ZEND_ACC_PUBLIC) {
        NULL, NULL, NULL
    }

};
/* }}} */

/** {{{
 */
PHP_MINIT_FUNCTION(ze_activestring) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "ZeActiveString", ze_activestring_methods);
    ze_activestring_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

    zend_declare_property_null(ze_activestring_ce, ZEND_STRL(ZE_ITEMS)   , ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(ze_activestring_ce, ZEND_STRL(ZE_ACTIVES) , ZEND_ACC_PRIVATE TSRMLS_CC);
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
