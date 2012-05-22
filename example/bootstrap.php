<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */



/**
 * 防止直接浏览
 */
define('ALLOW', 1);
/**
 * 网站根目录
 */
define('DIR_ROOT', str_replace('\\', '/', dirname(__FILE__)));
set_include_path(sprintf('%s%s.%s', DIR_ROOT, PATH_SEPARATOR, PATH_SEPARATOR . get_include_path()));

/**
 * 行结束符 直接使用 \n
 * （unix \n,win \r\n ,mac \r）
 */
define('EOL', "\n");
?>