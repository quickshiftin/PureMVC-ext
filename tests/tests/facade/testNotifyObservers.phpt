--TEST--
ensure Facade::notifyObservers passes the INotification instance to the view
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
$f = MyFacade::getInstance();
$f->notifyObservers(new Notification('blah'));
?>
--EXPECT--
Notification
