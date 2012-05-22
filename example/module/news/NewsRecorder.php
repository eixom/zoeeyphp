<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 *
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */


require_once 'module/common/Conn.php';

class NewsRecorder {

    public static function add($fields) {
        $id = 0;
        $info = null;
        do {
            $sql = 'INSERT INTO `news_e` (
                                  `id`,`title`,`category`,`detail`,`create_time`,`edit_time`
                        ) VALUES  (
                                  null,:title,:category,:detail,:create_time,:edit_time
                        )';

            $recorder = new ZeRecorder(Conn::getConn());

            $info = $recorder
                    ->query($sql)
                    ->bind($fields)
                    ->execute();
            if (!$info) {
                break;
            }
            $id = $recorder->lastId();
        } while (false);
        return $id;
    }

    public static function edit($fields) {
        $info = null;
        do {
            $sql = 'UPDATE `news_e` SET
                         `title` = :title
                        ,`category` = :category
                        ,`detail` = :detail
                        ,`edit_time` = :edit_time
                   WHERE 1=1
                   AND `id` = :id ';


            $recorder = new ZeRecorder(Conn::getConn());

            $info = $recorder
                    ->query($sql)
                    ->bind($fields)
                    ->exec();
        } while (false);
        return $info;
    }

    public static function modify($fields) {
        $info = null;
        do {
            $values = array();
            $types = array();
            /* @var $acStr ZeActiveString */
            $acStr = new ZeActiveString();
            $acStr->putAll(array(
                'update' => 'UPDATE `news_e`
                           SET `edit_time` = :edit_time '
                , 'update.category' => ',`category` = :category  '
                // update others,ex. IP,author...
                , 'where' => 'where 1 = 1 '
                , 'where.id' => 'AND FIND_IN_SET(`id`,:ids) > 0 '
                , 'where.state' => 'AND `state` < :state_del '
                , 'limit' => 'limit 20 '
            ));
            $acStr->active('update');
            $acStr->active('where');
            $acStr->active('limit');
            if (true) {

                $values['edit_time'] = $fields['edit_time'];
                $types['edit_time'] = PDO::PARAM_INT;

                $values['ids'] = join(',', $fields['id']);
                $types['ids'] = PDO::PARAM_STR;
                $acStr->active('where.id');
            }

            $category = $fields['category'];
            if (strlen($category) > 0) {
                $values['category'] = $category;
                $types['category'] = PDO::PARAM_INT;
                $acStr->active('update.category');
            }

            $recorder = new ZeRecorder(Conn::getConn());
            $sql = $acStr->toString();
            $info = $recorder
                    ->query($sql)
                    ->bind($values, $types)
                    ->execute();
        } while (false);
        return $info;
    }

    public static function del($fields) {
        $info = null;
        do {

            $sql = 'DELETE FROM news_e
                   WHERE 1=1
                   AND id = :id
                   /* LIMIT 1 */ ';

            $recorder = new ZeRecorder(Conn::getConn());

            $info = $recorder
                    ->query($sql)
                    ->bind($fields)
                    ->exec();
        } while (false);
        return $info;
    }

    /**
     *
     * @global UserInfo $userInfo
     * @param array $fields
     * @return array
     */
    public static function view($fields) {
        $info = null;
        do {
            $values = array();
            $types = array();
            $sql = 'SELECT
                          `id`,`title`,`category`
                          ,`detail`,`create_time`,`edit_time`
                   FROM `news_e`
                   WHERE `id` = :id
                  ';

            $values['id'] = $fields['id'];
            $types['id'] = PDO::PARAM_INT;

            $recorder = new ZeRecorder(Conn::getConn());

            $info = $recorder->query($sql)
                    ->bind($values)
                    ->fetch();
        } while (false);
        return $info;
    }

}

?>