<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */
/**
 * 配置文件
 */
class Config {
    const DOMAIN = '.zoeeyphp-example.com';
    const DEF_TIMEZONE = 'Asia/Shanghai';
    /**
     * database
     */
    const DB_LINK = 'sqlite:%s/database/zoeeyphp-example.db';
    /**
     * tpl cache
     */
    const TPL_COMPILE_CHCEK= true;
    const TPL_CACHE = false;
    const TPL_CACHE_EXPIRE = 3600;
    const TPL_DIR_MANAGE = '/tpl';
    const DIR_FILES = '/files';
}

define('IS_DEBUG', true);
define('IS_ACCESS', true);

define('PRIVATE_KEY', '10sda8sa7das7d6v6cx7v6s6da123jicd');

/* 状态级别 */
define('C_INIT', 'init');
define('C_SUCCESS', 'success');
define('C_ERROR', 'error');
define('C_WARN', 'warning');
?>
