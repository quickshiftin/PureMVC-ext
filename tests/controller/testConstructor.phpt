--TEST--
test the constructor to see if it initializes proxyMap, and base hook initializes View and calls the hook method
--FILE--
<?php
class MyController extends Controller {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function initializeController() {
		parent::initializeController();
		var_dump($this->view);
		var_dump($this->commandMap);
	}
}
MyController::getInstance();
?>
--EXPECT--
