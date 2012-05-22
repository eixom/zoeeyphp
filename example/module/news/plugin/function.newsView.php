<?php

/*
 * moxie(system128@gmail.com) 2011-12-29
 * http://code.google.com/p/zoeeyphp/
 * 
 * Code license: Apache License  Version 2.0
 * http://www.apache.org/licenses/LICENSE-2.0.txt
 */

/**
 * 单信息显示
 * @param type $params
 * @param Tpl $tpl
 * @return type
 */
function smarty_function_newsView($params, &$tpl) {
    $values = array();
    $types = array();
    /* @var $acStr ZeActiveString */
    $acStr = new ZeActiveString();
    $acStr->putAll(array(
        'select' => 'SELECT
                         `id`,`title`,`category`
                          ,`detail`,`create_time`,`edit_time`
                FROM `news_e` news
                WHERE 1=1
         '
        , 'where.id' => 'AND `id` = :id '
        , 'where.next' => 'AND `id` > :id '
        /* for demonstration */
        , 'where.category' => 'AND `category` = :category '
        , 'where.category.set' => 'AND FIND_IN_SET(`category`,:category) > 0 '
        , 'where.search' => 'AND (`title` LIKE "%" || :search || "%"
                                 OR `detail` LIKE "%" || :search || "%") '
        , 'limit' => 'LIMIT 1 '
    ));
    $acStr->active('select');
    $acStr->active('limit');

    $id = $tpl->getArg($params, 'id', null);
    if (is_numeric($id) && $id > 0) {
        $values['id'] = intval($id);
        $types['id'] = PDO::PARAM_INT;
        $acStr->active('where.id');
    }
    $next = $tpl->getArg($params, 'next', null);
    if (is_numeric($next) && $next > 0) {
        $values['id'] = intval($next);
        $types['id'] = PDO::PARAM_INT;
        $acStr->active('where.next');
    }
    /* todo prev */

    /* @var $recorder ZeRecorder */
    $recorder = new ZeRecorder(Conn::getConn());

    /* @var $pageSet ZePageSet */
    $pageSet = $tpl->getPageSet($params);

    $as = $tpl->getArg($params, 'as', 'news');
    $sql = $acStr->toString();

    $row = $recorder->query($sql)
            ->bind($values, $types)
            ->fetch();
    $tpl->assign($as, $row);
}

?>