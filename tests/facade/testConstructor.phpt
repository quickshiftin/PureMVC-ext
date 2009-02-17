--TEST--
ensure facade constructor calls $this->initializeFacade()
--FILE--
<?php
class F extends Facade {
	static public function getInstance() {
		if(is_null(self::$instance));
			self::$instance = new self();
		return self::$instance;
	}
	protected function initializeFacade() {
		parent::initializeFacade();
		echo 'Facade' . PHP_EOL;
	}
	protected function initializeModel() {
		parent::initializeModel();
		echo 'Model' . PHP_EOL;
	}
	protected function initializeView() {
		parent::initializeView();
		echo 'View' . PHP_EOL;
	}
	protected function initializeController() {
		parent::initializeController();
		echo 'Controller' . PHP_EOL;
	}
}
var_dump(F::getInstance());
?>
--EXPECT--
Model
Controller
View
Facade
object(F)#1 (3) {
  ["model:protected"]=>
  object(Model)#2 (1) {
    ["proxyMap:protected"]=>
    NULL
  }
  ["view:protected"]=>
  object(View)#4 (2) {
    ["mediatorMap:protected"]=>
    NULL
    ["observerMap:protected"]=>
    NULL
  }
  ["controller:protected"]=>
  object(Controller)#3 (2) {
    ["view:protected"]=>
    NULL
    ["commandMap:protected"]=>
    NULL
  }
}
