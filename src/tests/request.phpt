--TEST--
Check for ZeRequest
--SKIPIF--
<?php
if (!extension_loaded("zoeey")) {
    print "skip zoeey unload.";
}
$sapi_type = php_sapi_name();

if ($sapi_type != 'apache2handler' && substr($sapi_type, 0, 3) != 'cgi') {
    die('skip need CGI : ' . $sapi_type);
}
?>
--INI--
file_uploads=1
request_order="GP"
--GET--
name=example_get&age=128_get
--COOKIE--
name=example_cookie;age=128_cookie
--POST_RAW--
Content-type: multipart/form-data, boundary=AaB03x

--AaB03x
content-disposition: form-data; name="name"

example_post
--AaB03x
content-disposition: form-data; name="age"

128_post
--AaB03x
content-disposition: form-data; name="file_a"; filename="file1.txt"
Content-Type: text/plain

abcdef123456789
--AaB03x
content-disposition: form-data; name="file_b"; filename="file2.txt"
Content-Type: text/plain

abcdef123456789
--AaB03x--
--CGI--
--FILE--
<?php
require_once 'config.php';
session_start();
/* init */
$_SESSION['name'] = 'example_session';
$_SESSION['age'] = '128_session';
$_COOKIE['name'] = 'example_cookie';
$_COOKIE['age'] = '128_cookie';
setcookie('name', $_COOKIE['name'], time() + 3600);
setcookie('age', $_COOKIE['age'], time() + 3600);

$request = new ZeRequest();
if (php_sapi_name() == 'apache2handler') {
    echo '
<p>post twice<p>

<form action="?name=example_get&age=128_get" method="POST" enctype="multipart/form-data" >

    <input type="text" name="name" value="example_post" /><br />
    <input type="text" name="age" value="128_post" /><br />
    <input type="file" name="file_a" /><br />
    <input type="file" name="file_b" /><br />
    <input type="submit" value="test" />
</form>

';
}


/* getIp,fromIp */

echo 'getIp,fromIp:';
$_SERVER['HTTP_CLIENT_IP'] = null;
$_SERVER['HTTP_X_FORWARDED_FOR'] = '127.0.0.2,127.0.0.3';

echo assert('127.0.0.2' == $request->getIp(true));
echo ',', assert(2130706434 == $request->getIp());

$names = 'ip1 ,ip2';
$values = null;
$request->fromIp($values, $names);
echo ',', assert(array('ip1' => 2130706434, 'ip2' => 2130706434) == $values);

$values = new stdClass();
$request->fromIp($values, $names);

$expect = new stdClass();
$expect->ip1 = '2130706434';
$expect->ip2 = '2130706434';
echo ',', assert($values == $expect);

$names = array('ip5', 'ip6');
$values = null;
$_SERVER['HTTP_CLIENT_IP'] = '127.0.0.1';
$request->fromIp($values, $names);
echo ',', assert(array('ip5' => 2130706433, 'ip6' => 2130706433) == $values);

$values = new stdClass();
$request->fromIp($values, $names);
$expect = new stdClass();
$expect->ip5 = '2130706433';
$expect->ip6 = '2130706433';
echo ',', assert($values == $expect);


$_SERVER['HTTP_CLIENT_IP'] = '127.0.0.1';
echo ',', assert('127.0.0.1' == $request->getIp(true));
echo ',', assert(2130706433 == $request->getIp()), EOL;

/* isGet,isPost */
$request = new ZeRequest();
echo 'isGet,isPost:';
echo assert(!$request->isGet());
echo ',', assert($request->isPost());
echo ',', assert(!$request->isPut());
echo ',', assert(!$request->isDelete());
echo ',', assert(!$request->isTrace()), EOL;


/* getGet,fromGet */
echo 'getGet,fromGet:';
$name = 'name';
$request = new ZeRequest();
$value = $request->getGet($name);
echo assert($value == 'example_get');

$names = 'name, age';
$values = null;
$request = new ZeRequest();
$request->fromGet($values, $names);
echo ',', assert($values == array('name' => 'example_get', 'age' => '128_get'));

$values = new stdClass();
$request->fromGet($values, $names);
$expect = new stdClass();
$expect->name = 'example_get';
$expect->age = '128_get';
echo ',', assert($values == $expect), EOL;

/* getPost,fromPost */
echo 'getPost,fromPost:';

$name = 'name';
$request = new ZeRequest();
$value = $request->getPost($name);
echo assert($value == 'example_post');

$names = 'name, age';
$values = null;
$request = new ZeRequest();
$request->fromPost($values, $names);
echo ',', assert($values == array('name' => 'example_post', 'age' => '128_post')), EOL;

/* getCookie,fromCookie */
echo 'getCookie,fromCookie:';

$name = 'name';
$request = new ZeRequest();
$value = $request->getCookie($name);

echo assert($value == 'example_cookie');

$names = 'name, age';
$values = null;
$request = new ZeRequest();
$request->fromCookie($values, $names);
echo ',', assert($values == array('name' => 'example_cookie', 'age' => '128_cookie')), EOL;

/* getRequest,fromRequest */
echo 'getRequest,fromRequest:';

$name = 'name';
$request = new ZeRequest();
$value = $request->getRequest($name);
echo assert($value == 'example_cookie');

$names = 'name, age';
$values = null;
$request = new ZeRequest();
$request->fromRequest($values, $names);
echo ',', assert($values == array('name' => 'example_cookie', 'age' => '128_cookie')), EOL;

/* getServer,fromServer */
echo 'getServer,fromServer:';

$name = 'SCRIPT_FILENAME';
$request = new ZeRequest();
$value = $request->getServer($name);

echo assert($value == __FILE__);

$names = 'REQUEST_METHOD, SCRIPT_FILENAME';
$values = null;
$request = new ZeRequest();
$request->fromServer($values, $names);

echo ',', assert($values == array('REQUEST_METHOD' => 'POST', 'SCRIPT_FILENAME' => __FILE__)), EOL;

/* getFile,fromFile */
echo 'getFile,fromFile:';

$name = 'file_a';
$request = new ZeRequest();
$value = $request->getFile($name);
echo assert($value['error'] === 0);

$names = 'file_a, file_b';
$values = null;
$request = new ZeRequest();
$request->fromFile($values, $names);
echo ',', assert($values['file_a']['error'] === 0);
echo ',', assert($values['file_b']['error'] === 0), EOL;

/* getSession,fromSession */
echo 'getSession,fromSession:';

/* getSession */
$name = 'name';
$request = new ZeRequest();
$value = $request->getSession($name);

echo assert($value == 'example_session');

/* fromSession */
$names = 'name, age';
$values = array();
$request = new ZeRequest();
$request->fromSession($values, $names);
echo ',', assert($values['name'] == 'example_session');
echo ',', assert($values['age'] == '128_session'), EOL;

/* setValues */
echo 'setValues:';
// article.php/age/821_router/search/test_search?page=128_get
$request = new ZeRequest();
$request->setValues(array('age' => '321_router', 'search' => 'test_search'));

$value = $request->getGet('age');
echo assert($value == '128_get');

$value = $request->getGet('search');
echo ',', assert($value == 'test_search');

$values = array();
$request->fromGet($values, 'name,age,search');
echo ',', assert($values['name'] == 'example_get');
echo ',', assert($values['age'] == '128_get');
echo ',', assert($values['search'] == 'test_search');

$value = $request->getRequest('search');
echo ',', assert($value == 'test_search');

$values = array();
$request->fromRequest($values, 'name,age,search');
echo ',', assert($values['name'] == 'example_cookie');
echo ',', assert($values['age'] == '128_cookie');
echo ',', assert($values['search'] == 'test_search');
?>
--EXPECT--
getIp,fromIp:1,1,1,1,1,1,1,1
isGet,isPost:1,1,1,1,1
getGet,fromGet:1,1,1
getPost,fromPost:1,1
getCookie,fromCookie:1,1
getRequest,fromRequest:1,1
getServer,fromServer:1,1
getFile,fromFile:1,1,1
getSession,fromSession:1,1,1
setValues:1,1,1,1,1,1,1,1,1
