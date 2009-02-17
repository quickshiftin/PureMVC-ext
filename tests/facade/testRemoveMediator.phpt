--TEST--
ensure Facade::removeMediator passes the $mediatorName to the view
--FILE--
<?php
class MyView extends View {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}
	public function removeMediator($notificationName) {
		echo $notificationName . PHP_EOL;
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
$f = MyFacade::getInstance();
$f->removeMediator('MyNotification');
?>
--EXPECT--
MyNotification
