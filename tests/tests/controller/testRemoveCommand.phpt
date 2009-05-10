--TEST--
after registering a command, try to remove it
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyController::disableEcho();
MyController::enableMyView();
$c = MyController::getInstance();
$c->registerCommand('testNotification', 'MySimpleCommand');
$c->removeCommand('testNotification');
var_dump($c->hasCommand('testNotification'));
--EXPECT--
bool(false)
