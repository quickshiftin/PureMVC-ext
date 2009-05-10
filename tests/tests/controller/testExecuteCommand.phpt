--TEST--
lets see if we call the execute() method and ensure it gets passed the INotification
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyController::disableEcho();
//// TODO; test w/ native View ..
MyController::enableMyView();
$c = MyController::getInstance();
/// TODO: ANOTHER TEST AGAINST THIS VAL
//$c->registerCommand('testNotification', 'MySimpleCommand');
$c->registerCommand('MyNotification', 'MySimpleCommand');
MyNotification::dissableNameOverride();
$c->executeCommand(new MyNotification());
?>
--EXPECT--
MyNotification
