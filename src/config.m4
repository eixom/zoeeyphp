dnl $Id$
dnl config.m4 for extension zoeey

PHP_ARG_WITH(zoeey, for zoeey support,
[  --with-zoeey             Include zoeey support])

if test "$PHP_ZOEEY" != "no"; then


AC_DEFUN([PHP_ZOEEY_ADD_SOURCE], [
    PHP_ZOEEY_SOURCES="$PHP_ZOEEY_SOURCES $1"
])
    PHP_ZOEEY_ADD_SOURCE([zoeey.c helper.c recorder.c status.c])
    PHP_ZOEEY_ADD_SOURCE([activestring.c router.c pageset.c request.c])
    PHP_ZOEEY_ADD_SOURCE([functions.c])

    PHP_NEW_EXTENSION(zoeey, $PHP_ZOEEY_SOURCES,$ext_shared)

    ifdef([PHP_ADD_EXTENDION_DEP],
    [
        PHP_ADD_EXTENSION_DEP(PDO, pcre, session, true)
    ])

fi
# vim600: sts=4 sw=4 et
