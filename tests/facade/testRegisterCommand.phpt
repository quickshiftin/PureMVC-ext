--TEST--
ensure we can store a command via Facade::registerCommand
--FILE--
<?php
class MyController extends Controller {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function registerCommand($notificationName, $commandClassRef) {
		echo "$notificationName $commandClassRef" . PHP_EOL;
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
$f->registerCommand('SubCommand', 'MyNotification');
?>
--EXPECT--
SubCommand MyNotification
