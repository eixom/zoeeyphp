<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

class Vali {

    /**
     *  check title
     *
     * @param string $title
     * @return bool
     */
    public static function title($title) {
        $len = strlen($title); // mb_strlen is better
        if ($len > 3 && $len < 300) {
            return true;
        }
        return false;
    }

    /**
     *  check detail
     *
     * @param string $detail
     * @return bool
     */
    public static function detail($detail) {
        $len = strlen($detail); // mb_strlen is better
        if ($len > 3 && $len < 5000) {
            return true;
        }
        return false;
    }

    /**
     *  check positive integer
     *
     * @param int $num
     * @return bool
     */
    public static function positive($num) {

        if (!preg_match('/^[0-9]+$/', $num)) {
            return false;
        }

        if ($num > 0) {
            return true;
        }

        return false;
    }

    /**
     *  check orderBy
     *
     * @param string $orderBy
     * @return bool
     */
    public static function orderBy($orderBy) {
        if (in_array($orderBy, array('creat_time.asc', 'creat_time.desc', 'edit_time.asc', 'edit_time.desc'))) {
            return true;
        }
        return false;
    }

}

?>
