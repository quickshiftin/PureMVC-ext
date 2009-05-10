--TEST--
lest see if the command gets added to the commandMap & that the observer is registered w/ the View
ensure the Controller calls registerObserver on its composed View
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
MyView::makeVerbose();
MyController::disableEcho();
MyController::enableMyView();
$c = MyController::getInstance();
$c->registerCommand('testNotification', 'MySimpleCommand');
?>
--EXPECT--
array(2) {
  [0]=>
  string(16) "testNotification"
  [1]=>
  object(Observer)#3 (2) {
    ["notify:private"]=>
    string(14) "executeCommand"
    ["context:private"]=>
    object(MyController)#1 (2) {
      ["view:protected"]=>
      object(MyView)#2 (2) {
        ["mediatorMap:protected"]=>
        array(0) {
        }
        ["observerMap:protected"]=>
        array(0) {
        }
      }
      ["commandMap:protected"]=>
      array(1) {
        ["testNotification"]=>
        string(15) "MySimpleCommand"
      }
    }
  }
}
