dnl $Id$
dnl config.m4 for extension zoeey

PHP_ARG_WITH(zoeey, for zoeey support,
[  --with-zoeey             Include zoeey support])

if test "$PHP_ZOEEY" != "no"; then

	PHP_NEW_EXTENSION(zoeey, zoeey.c helper.c recorder.c status.c activestring.c \
							  router.c pageset.c loader.c functions.c\
							  ,$ext_shared)

	ifdef([PHP_ADD_EXTENDION_DEP],
	[
		PHP_ADD_EXTENSION_DEP(PDO, pcre, session, true)
	])

fi
