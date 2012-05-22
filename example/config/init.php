<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 * 
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

require_once 'config/Config.php';

if (IS_DEBUG) {
    error_reporting(E_ALL | E_STRICT);
} else {
    error_reporting(0);
}

require_once 'lib/main/funces.php';
require_once 'lib/firephp/fb.php';
require_once 'lib/main/Supervisor.php';

date_default_timezone_set(Config::DEF_TIMEZONE);
/**
 * set session handler here
 * ini_set('session.save_handler', 'memcache');
 * ini_set('session.save_path', Config::MMC_URL);
 */
session_cache_expire(300);
session_set_cookie_params(3600 * 24 * 7 * 2, '/', Config::DOMAIN);
?>