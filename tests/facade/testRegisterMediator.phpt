--TEST--
ensure we can store a command via Facade::registerMediator
--FILE--
<?php
class MyView extends View {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function registerMediator($mediator) {
		var_dump($mediator);
	}
}
class MyFacade extends Facade {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	protected function initializeView() {
		if(is_null($this->view))
			$this->view = MyView::getInstance();
	}
}
$f = Facade::getInstance();
$f->registerMediator(new Mediator());
--EXPECT--
