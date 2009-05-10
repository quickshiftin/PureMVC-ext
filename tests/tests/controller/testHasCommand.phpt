--TEST--
ensure the Controller tells us we have a command once its registered
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyController::disableEcho();
MyController::enableMyView();
$c = MyController::getInstance();
$c->registerCommand('MyNotification', 'MySimpleCommand');
var_dump($c->hasCommand('MyNotification'));
--EXPECT--
bool(true)
