--TEST--
ensure Facade::sendNotification correctly creates Notification
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$f = MyFacade::getInstance();
$f->sendNotification('blah');
$f->sendNotification('blah', 'testbody');
$f->sendNotification('blah', 'testbody', 'testtype');
?>
--EXPECT--
Notification
Notification
Notification
