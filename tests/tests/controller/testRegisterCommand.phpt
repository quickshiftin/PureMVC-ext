--TEST--
lest see if the command gets added to the commandMap & that the observer is registered w/ the View
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
$c = Controller::getInstance();
$c->registerCommand('testNotification', 'MySimpleCommand');
var_dump($c);
var_dump(View::getInstance());
?>
--EXPECT--
sadf
