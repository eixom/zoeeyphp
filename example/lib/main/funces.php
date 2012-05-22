<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 * 
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 * 输出 html utf-8 头
 */
function header_html() {
    header('Content-Type:text/html; charset=utf-8');
}

function encrypt($psw) {
    return md5(sprintf("%s_%s", PRIVATE_KEY, $psw));
}

/**
 * 跳转至
 * @param string $url
 */
function redirect($url) {
    header('location:' . $url, 307);
    exit();
}

?>