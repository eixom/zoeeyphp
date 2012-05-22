<?php

$config = array();
$dir = dirname(__FILE__);
define('DIR_TEST', $dir);

/**
 * mysql connection
 * for recorder.phpt
 */
function get_conn() {

    $conn = new PDO('sqlite:' . DIR_TEST . '/foo.db');

    $conn->exec('SET NAMES utf8');
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    return $conn;
}

function clean_db() {
    unlink(DIR_TEST . '/foo.db');
}

error_reporting(E_ALL | E_STRICT);

define('EOL', "\n");
?>