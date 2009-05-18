--TEST--
lets see if notifyObservers calls the right method on the context and passes in the notification
--FILE--
<?php
include(dirname(__FILE__) . '/../../testlib/include.php');
class Something {
	public function notify($notification) {
		if($notification->getName() == 'testEvent')
			echo 'testEvent recieved' . PHP_EOL;
	}
}
$o = new Observer('notify', new Something());
$o->notifyObserver(new Notification('testEvent'));
?>
--EXPECT--
testEvent recieved
