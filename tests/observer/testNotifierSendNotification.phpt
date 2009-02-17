--TEST--
lets see if we can call the Facade::sendNotification successfully
--FILE--
<?php
class MyFacade extends Facade {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function notifyObservers($notification) {
		var_dump($notification);
	}
}
class MyNotifier extends Notifier {
	public function __construct() {
		$this->facade = MyFacade::getInstance();
	}
}
$n = new MyNotifier();
$n->sendNotification('blah');
$n->sendNotification('blah', 'testbody');
$n->sendNotification('blah', 'testbody', 'testtype');
?>
--EXPECT--
object(Notification)#6 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(0) ""
}
object(Notification)#7 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(0) ""
  ["body:private"]=>
  string(8) "testbody"
}
object(Notification)#8 (3) {
  ["name:private"]=>
  string(4) "blah"
  ["type:private"]=>
  string(8) "testtype"
  ["body:private"]=>
  string(8) "testbody"
}
