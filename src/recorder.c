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
#include "ext/pdo/php_pdo.h"
#include "ext/standard/php_smart_str.h"
#include "php_zoeey.h"

#include "helper.h"
#include "recorder.h"

#define ZE_PDO_DBH    "conn"
#define ZE_STMT       "stmt"
#define ZE_KEYS       "keys"

/** {{{ current pageset object
 */
zend_class_entry * ze_recorder_ce;
/* }}} */

/** {{{ to_indexed
*/
static zval * to_indexed(zval * keys, char * query, int len) {
	zval * sql = NULL;
	char   ch  = 0;
	int    i   = 0;
	smart_str skey       = {NULL, 0, 0};
	smart_str ssql       = {NULL, 0, 0};

	for(i=0; i<len; i++) {
		ch = *(query + i);
		if (ch == ':') {
			for (i++; i < len; i++) {
				ch = *(query + i);
				/* [a-zA-Z0-9_]+ */
				if ((ch > 96 && ch < 123)
						|| (ch > 64 && ch < 91)
						|| (ch > 47 && ch < 58)
						|| ch == 95
				   ) {
					smart_str_appendc(&skey, ch);
				} else {
					break;
				}
			}
			smart_str_appendc(&ssql, '?');
			smart_str_appendc(&ssql, ch);

			smart_str_0(&skey);

			if (ssql.len) {
				add_next_index_stringl(keys, skey.c, skey.len, 1);
			}
			smart_str_free(&skey);
		} else {
			smart_str_appendc(&ssql, ch);
		}
	}

	MAKE_STD_ZVAL(sql);
	if (ssql.len) {
		ZVAL_STRINGL(sql, ssql.c, ssql.len, 1);
	}else{
		ZVAL_STRINGL(sql, "", 0, 1);
	}

	smart_str_free(&skey);
	smart_str_free(&ssql);

	return sql;
}
/*}}}*/

/** {{{ public ZeRecorder::__construct()
 */
PHP_METHOD(ze_recorder, __construct) {
	zval * self                   = NULL;
	zval * pdo_dbh                = NULL;
	zval * keys                   = NULL;
	zval * stmt                   = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O"
							 , &pdo_dbh
							 , php_pdo_get_dbh_ce()
							 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	self = getThis();

	if (pdo_dbh && Z_TYPE_P(pdo_dbh) == IS_OBJECT) {
		zend_update_property(ze_recorder_ce, self, ZEND_STRL(ZE_PDO_DBH) , pdo_dbh TSRMLS_CC);
	} else {
		/* PDO is not connected */
		ze_error(E_ERROR TSRMLS_CC, "recorder.construct.pdo_dbh.not_object");
	}

}

/* }}} */

/** {{{ public ZeRecorder::query($sql = null, $args = null, $_ = null)
 */
PHP_METHOD(ze_recorder, query) {
	zval *   self      = NULL;
	zval *   conn      = NULL;
	zval *   stmt      = NULL;
	char *   query     = NULL;
	int      query_len = 0;
	zval *   sql       = NULL;
	zval *   keys      = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s"
				, &query
				, &query_len
				) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* call prepare */

	self = getThis();

	conn = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_PDO_DBH), 0 TSRMLS_CC);

	do {
		if (!conn || Z_TYPE_P(conn) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.query.conn.not_object");
			break;
		}

		MAKE_STD_ZVAL(keys);
		array_init(keys);

		sql = to_indexed(keys, query, query_len);

		if (!sql) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.query.sql.is_empty");
			break;
		}

		zend_call_method(&conn, Z_OBJCE_P(conn), NULL, ZEND_STRL("prepare"), &stmt, 1, sql, NULL TSRMLS_CC);

		if (!stmt || EG(exception)) {
			zval_dtor(keys);
			FREE_ZVAL(keys);
			ze_error(E_ERROR TSRMLS_CC, "recorder.query.stmt.not_object");
			break;
		}
		zend_update_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), stmt TSRMLS_CC);
		zend_update_property(ze_recorder_ce, self, ZEND_STRL(ZE_KEYS), keys TSRMLS_CC);

		if (stmt != NULL) {
			zval_ptr_dtor(&stmt);
		}

		if (keys != NULL) {
			zval_ptr_dtor(&keys);
		}

	} while (0);


	if (sql != NULL) {
		zval_dtor(sql);
		FREE_ZVAL(sql);
	}

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRecorder::getStmt()
 */
PHP_METHOD(ze_recorder, getStmt) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	RETURN_ZVAL(stmt, 1, 0);
}
/* }}} */

/** {{{ public ZeRecorder::bind()
 */
 //@todo
PHP_METHOD(ze_recorder, bind) {
	zval *   self                = NULL;
	zval *   stmt                = NULL;
	zval *   fields              = NULL;
	zval *   types               = NULL;
	HashTable * keys_table       = NULL;
	HashPosition keys_pointer    = NULL;
	HashTable * fields_table     = NULL;
	HashTable * types_table      = NULL;
	zval ** val                  = NULL;
	zval *  val_copy             = NULL;
	zval ** type                 = NULL;
	zval *  type_copy            = NULL;
	zval *  keys                 = NULL;
	zval ** key                  = NULL;
	int     pos                  = 0;
	zval *  pos_z                = NULL;
	zval ** params[3];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z"
							, &fields
							, &types
							) == FAILURE) {
		WRONG_PARAM_COUNT;
	}

	if (Z_TYPE_P(fields) == IS_OBJECT && Z_OBJ_HT_P(fields)->get_properties != NULL) {
		fields_table = Z_OBJ_HT_P(fields)->get_properties(fields TSRMLS_CC);

	} else if (Z_TYPE_P(fields) == IS_ARRAY) {
		fields_table   = Z_ARRVAL_P(fields);
	}

	if (!fields_table) {
		/* need array or object for '$fields' . */
		ze_error(E_ERROR TSRMLS_CC, "recorder.bind.fields.type");
		return;
	}

	if (types) {
		if (Z_TYPE_P(fields) == IS_OBJECT && Z_OBJ_HT_P(types)->get_properties != NULL) {
			types_table = Z_OBJ_HT_P(types)->get_properties(types TSRMLS_CC);

		} else if (Z_TYPE_P(fields) == IS_ARRAY) {
			types_table    = Z_ARRVAL_P(types);

		}

		if (!types_table){
			/* need array or object for '$types' . */
			ze_error(E_ERROR TSRMLS_CC, "recorder.bind.types.type");
			return;
		}
	}

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);
	keys = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_KEYS), 0 TSRMLS_CC);

	do {

		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			 ze_error(E_ERROR TSRMLS_CC, "recorder.bind.stmt.null");
			 break;
		}

		if (!keys || Z_TYPE_P(keys) != IS_ARRAY) {
			 ze_error(E_ERROR TSRMLS_CC, "recorder.bind.keys.is_array");
			 break;
		}

		keys_table = Z_ARRVAL_P(keys);
		pos = 0;

		for (zend_hash_internal_pointer_reset_ex(keys_table, &keys_pointer);
				zend_hash_get_current_data_ex(keys_table,(void**) &key, &keys_pointer) == SUCCESS;
				zend_hash_move_forward_ex(keys_table, &keys_pointer)) {

			if (zend_hash_find(fields_table, Z_STRVAL_PP(key), Z_STRLEN_PP(key) + 1, (void **) &val) == SUCCESS) {
				pos++;
				ALLOC_INIT_ZVAL(pos_z);
				ZVAL_LONG(pos_z, pos);

				val_copy = *val;
				zval_copy_ctor(val_copy);

				if (types_table
					&& zend_hash_find(types_table, Z_STRVAL_PP(key), Z_STRLEN_PP(key) + 1, (void **) &type)  == SUCCESS) {

					type_copy = *type;
					zval_copy_ctor(type_copy);

					params[0] = &pos_z;
					params[1] = &val_copy;
					params[2] = &type_copy;

					ze_call_method(&stmt, NULL, NULL, ZEND_STRL("bindValue"), NULL, 3, params TSRMLS_CC);

				}else{
					zend_call_method(&stmt, NULL, NULL, ZEND_STRL("bindValue"), NULL, 2, pos_z, val_copy TSRMLS_CC);
				}

				zval_dtor(pos_z);
				FREE_ZVAL(pos_z);
			}
		}
	} while (0);

	RETURN_ZVAL(self, 1, 0);
}
/* }}} */

/** {{{ public ZeRecorder::fetch()
 */
PHP_METHOD(ze_recorder, fetch) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	long     style       = 0;
	zval *   style_z     = NULL;
	zval *   success     = NULL;
	zval *   row         = NULL;

	/* PDO::FETCH_ASSOC */
	style   =   2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l"
							 , &style
							 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* call execute */
	/* call fetch */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	do {
		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.fetch.stmt.not_object");
			break;
		}

		zend_call_method(&stmt, Z_OBJCE_P(stmt), NULL, ZEND_STRL("execute"), &success, 0, NULL, NULL TSRMLS_CC);

		if (!success || EG(exception)) {
			zval_dtor(success);
			FREE_ZVAL(success);
			break;
		}

		convert_to_boolean(success);

		if (Z_BVAL_P(success) == 1) {
			ALLOC_INIT_ZVAL(style_z);
			ZVAL_LONG(style_z, style);
			zend_call_method(&stmt, NULL, NULL, ZEND_STRL("fetch"), &row, 1, style_z, NULL TSRMLS_CC);
			zval_dtor(style_z);
			FREE_ZVAL(style_z);
		}

		zval_dtor(success);
		FREE_ZVAL(success);

	} while (0);

	if (row) {
		RETURN_ZVAL(row, 1, 1);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/** {{{ public ZeRecorder::getInt()
 */
PHP_METHOD(ze_recorder, getInt) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	zval *   success     = NULL;
	zval *   row         = NULL;
	long     style       = 0;
	zval *   style_z     = NULL;
	long     count       = 0;
	zval **  count_z     = NULL;


	/* PDO::FETCH_NUM */
	style   =   3;

	/* call execute */
	/* call fetch */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);
	do {
		if (!stmt) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.fetch.stmt.null");
			break;
		}

		if (Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.fetch.stmt.not_object");
			break;
		}

		zend_call_method(&stmt, Z_OBJCE_P(stmt), NULL, ZEND_STRL("execute"), &success, 0, NULL, NULL TSRMLS_CC);

		if (!success || EG(exception)) {
			break;
		}

		convert_to_boolean(success);

		if (Z_BVAL_P(success) == 1) {
			ALLOC_INIT_ZVAL(style_z);
			ZVAL_LONG(style_z, style);
			zend_call_method(&stmt, NULL, NULL, ZEND_STRL("fetch"), &row, 1, style_z, NULL TSRMLS_CC);
			zval_dtor(style_z);
			FREE_ZVAL(style_z);
		}

		zval_dtor(success);
		FREE_ZVAL(success);

		if (!row) {
			break;
		}

		if (Z_TYPE_P(row) == IS_ARRAY
				&& zend_hash_has_more_elements(Z_ARRVAL_P(row)) == SUCCESS
				&& zend_hash_get_current_data(Z_ARRVAL_P(row), (void **) &count_z) == SUCCESS) {

				convert_to_long_ex(count_z);
				count = Z_LVAL_PP(count_z);

				zval_dtor(*count_z);
				FREE_ZVAL(*count_z);
			}

		zval_dtor(row);
		FREE_ZVAL(row);
	} while (0);

	RETURN_LONG(count);
}
/* }}} */

/** {{{ public ZeRecorder::fetchAll()
 */
PHP_METHOD(ze_recorder, fetchAll) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	long     style       = 0;
	zval *   style_z     = NULL;
	zval *   success     = NULL;
	zval *   row         = NULL;

	/* PDO::FETCH_ASSOC */
	style   =   2;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l"
							 , &style
							 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* call execute */
	/* call fetch */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	do {
		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.fetch.stmt.not_object");
			break;
		}

		zend_call_method(&stmt, Z_OBJCE_P(stmt), NULL, ZEND_STRL("execute"), &success, 0, NULL, NULL TSRMLS_CC);

		if (!success || EG(exception)) {
			break;
		}

		convert_to_boolean(success);

		if (Z_BVAL_P(success) == 1) {
			ALLOC_INIT_ZVAL(style_z);
			ZVAL_LONG(style_z, style);
			zend_call_method(&stmt, NULL, NULL, ZEND_STRL("fetchAll"), &row, 1, style_z, NULL TSRMLS_CC);
			zval_dtor(style_z);
			FREE_ZVAL(style_z);
		}

		zval_dtor(success);
		FREE_ZVAL(success);
	} while (0);

	if (row) {
		RETURN_ZVAL(row, 1, 1);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/** {{{ public ZeRecorder::execute()
 */
PHP_METHOD(ze_recorder, execute) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	zval *   success     = NULL;

	/* call execute */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	do {
		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.fetch.stmt.not_object");
			break;
		}

		zend_call_method(&stmt, Z_OBJCE_P(stmt), NULL, ZEND_STRL("execute"), &success, 0, NULL, NULL TSRMLS_CC);

		if (!success || EG(exception)) {
			break;
		}

		convert_to_boolean(success);

	} while (0);

	if (!success){
	   RETURN_FALSE;
	}
	if (Z_BVAL_P(success) == 1) {
		RETVAL_TRUE;
	}else{
		RETVAL_FALSE;
	}

	zval_dtor(success);
	FREE_ZVAL(success);

}
/* }}} */

/** {{{ public ZeRecorder::exec()
 */
PHP_METHOD(ze_recorder, exec) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	zval *   success     = NULL;
	zval *   row_count    = NULL;

	/* call execute */
	/* call rowCount */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	do {
		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.exec.stmt.not_object");
			break;
		}

		zend_call_method(&stmt, Z_OBJCE_P(stmt), NULL, ZEND_STRL("execute"), &success, 0, NULL, NULL TSRMLS_CC);

		if (!success || EG(exception)) {
			break;
		}

		convert_to_boolean(success);

		if (Z_BVAL_P(success) == 1) {
			zend_call_method(&stmt, NULL, NULL, ZEND_STRL("rowCount"), &row_count, 0, NULL, NULL TSRMLS_CC);
		}

		zval_dtor(success);
		FREE_ZVAL(success);

	} while (0);

	if (row_count) {
		 convert_to_long(row_count);
		 RETURN_ZVAL(row_count, 1, 1);
	} else {
		RETURN_FALSE;
	}

}
/* }}} */

/** {{{ public ZeRecorder::lastId()
 */
PHP_METHOD(ze_recorder, lastId) {
	zval *   self        = NULL;
	zval *   conn        = NULL;
	char *   name        = NULL;
	int      name_len    = 0;
	zval *   name_z      = NULL;
	zval *   lastid      = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s"
							 , &name, &name_len
							 ) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
	/* call lastInsertId */

	self = getThis();

	conn = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_PDO_DBH), 0 TSRMLS_CC);

	do {
		if (!conn || Z_TYPE_P(conn) != IS_OBJECT) {

			ze_error(E_ERROR TSRMLS_CC, "recorder.lastId.conn.not_object");
			break;
		}

		ALLOC_INIT_ZVAL(name_z);
		ZVAL_STRINGL(name_z, name, name_len, 1);
		zend_call_method(&conn, NULL, NULL, ZEND_STRL("lastInsertId"), &lastid, 1, name_z, NULL TSRMLS_CC);
		zval_dtor(name_z);
		FREE_ZVAL(name_z);

	} while (0);


	if (lastid) {
		 convert_to_long(lastid);
		 RETURN_ZVAL(lastid, 1, 1);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/** {{{ public ZeRecorder::affected()
 */
PHP_METHOD(ze_recorder, affected) {
	zval *   self        = NULL;
	zval *   stmt        = NULL;
	zval *   row_count      = NULL;

	/* call row_count */

	self = getThis();

	stmt = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_STMT), 0 TSRMLS_CC);

	do {
		if (!stmt || Z_TYPE_P(stmt) != IS_OBJECT) {
			ze_error(E_ERROR TSRMLS_CC, "recorder.affected.stmt.not_object");
			break;
		}


		zend_call_method(&stmt, NULL, NULL, ZEND_STRL("rowCount"), &row_count, 0, NULL, NULL TSRMLS_CC);

	} while (0);

	if (row_count) {
		 convert_to_long(row_count);
		 RETURN_ZVAL(row_count, 1, 1);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/** {{{ CONN_METHOD
 */
#define CONN_METHOD(method_name)                                               \
	zval *   self        = NULL;                                               \
	zval *   conn        = NULL;                                               \
	zval *   success     = NULL;                                               \
																			   \
	/* call beginTransaction */                                                \
																			   \
	self = getThis();                                                          \
																			   \
	conn = zend_read_property(ze_recorder_ce, self, ZEND_STRL(ZE_PDO_DBH), 0 TSRMLS_CC);\
																			   \
	do {                                                                       \
		if (!conn || Z_TYPE_P(conn) != IS_OBJECT) {                            \
																			   \
			ze_error(E_ERROR TSRMLS_CC, "recorder." method_name ".conn.not_object"); \
			break;                                                             \
		}                                                                      \
																			   \
		zend_call_method(&conn, NULL, NULL, ZEND_STRL(method_name), &success, 0, NULL, NULL TSRMLS_CC);\
																			   \
		if (!success || EG(exception)) {                                       \
			ze_error(E_ERROR TSRMLS_CC, "recorder." method_name ".conn.not_object"); \
			break;                                                             \
		}                                                                      \
																			   \
		convert_to_boolean(success);                                           \
																			   \
	} while (0);                                                               \
																			   \
	if (!success){                                                             \
	   RETURN_FALSE;                                                           \
	}                                                                          \
	if (Z_BVAL_P(success) == 1) {                                              \
		RETVAL_TRUE;                                                           \
	}else{                                                                     \
		RETVAL_FALSE;                                                          \
	}                                                                          \
	if (success) {                                                             \
		zval_dtor(success);                                                    \
		FREE_ZVAL(success);                                                    \
	}
/*}}}*/

/** {{{ public ZeRecorder::begin()
 */
PHP_METHOD(ze_recorder, begin) {
   CONN_METHOD("beginTransaction");
}
/* }}} */

/** {{{ public ZeRecorder::rollback()
 */
PHP_METHOD(ze_recorder, rollback) {
	CONN_METHOD("rollback");
}
/* }}} */

/** {{{ public ZeRecorder::commit()
 */
PHP_METHOD(ze_recorder, commit) {
	CONN_METHOD("commit");
}
/* }}} */

/** {{{ public ZeRecorder::debugDump()
 */
PHP_METHOD(ze_recorder, debugDump) {
	CONN_METHOD("debugDumpParams");
}
/* }}} */

/** {{{ public ZeRecorder::close()
 */
PHP_METHOD(ze_recorder, close) {

}
/* }}} */

/** {{{ construct PDO $pdo
 */
ZEND_BEGIN_ARG_INFO(recorder_construct_args, 0)
	ZEND_ARG_OBJ_INFO(0, pdo, PDO, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $sql = null, $args = null, $_ = null
 */
ZEND_BEGIN_ARG_INFO(recorder_query_args, 0)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $fields, $types = array(), $flips = null
 */
ZEND_BEGIN_ARG_INFO(recorder_bind_args, 0)
	ZEND_ARG_INFO(0, fields)
	ZEND_ARG_INFO(0, types)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $style = PDO::FETCH_ASSOC
 */
ZEND_BEGIN_ARG_INFO(recorder_fetch_args, 0)
	ZEND_ARG_INFO(0, style)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $style = PDO::FETCH_ASSOC
 */
ZEND_BEGIN_ARG_INFO(recorder_fetchAll_args, 0)
	ZEND_ARG_INFO(0, style)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ $name = null
 */
ZEND_BEGIN_ARG_INFO(recorder_lastId_args, 0)
	ZEND_ARG_INFO(0, style)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ methods
 */
zend_function_entry ze_recorder_methods[] = {
	PHP_ME(ze_recorder, __construct     , recorder_construct_args  , ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(ze_recorder, query           , recorder_query_args      , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, getStmt         , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, bind            , recorder_bind_args       , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, fetch           , recorder_fetch_args      , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, getInt          , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, fetchAll        , recorder_fetchAll_args   , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, execute         , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, exec            , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, lastId          , recorder_lastId_args     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, affected        , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, begin           , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, rollback        , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, commit          , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, close           , NULL                     , ZEND_ACC_PUBLIC)
	PHP_ME(ze_recorder, debugDump       , NULL                     , ZEND_ACC_PUBLIC)
	PHP_MALIAS(ze_recorder, __destruct  , close      , NULL        , ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ze_recorder) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "ZeRecorder", ze_recorder_methods);
	ze_recorder_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(ze_recorder_ce, ZEND_STRL(ZE_PDO_DBH) , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_recorder_ce, ZEND_STRL(ZE_STMT) , ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ze_recorder_ce, ZEND_STRL(ZE_KEYS) , ZEND_ACC_PRIVATE TSRMLS_CC);

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
