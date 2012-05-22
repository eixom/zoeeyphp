<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 * 数据库连接
 */
class Conn {

    private static $conn = null;

    /**
     * 数据库连接
     */
    public static function &getConn() {
        if (self::$conn == null) {
            $conn = new PDO(sprintf(Config::DB_LINK, DIR_ROOT), "charset=UTF-8");

            if (IS_DEBUG) {
                $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            } else {
                $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_SILENT);
            }
            self::$conn = $conn;
        }
        return self::$conn;
    }

    /**
     * 禁止克隆
     */
    public function __clone() {
        trigger_error('Clone is not allowed.', E_USER_ERROR);
    }

}

?>