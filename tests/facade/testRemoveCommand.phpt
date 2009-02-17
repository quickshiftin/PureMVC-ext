--TEST--
ensure Facade::removeCommand passes the $commandName to the controller
--FILE--
<?php
class MyController extends Controller {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function removeCommand($notificationName) {
		echo $notificationName . PHP_EOL;
	}
}
class MyFacade extends Facade {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	protected function initializeController() {
		if(is_null($this->controller))
			$this->controller = MyController::getInstance();
	}
}
$f = MyFacade::getInstance();
$f->removeCommand('MyNotification');
?>
--EXPECT--
MyNotification
