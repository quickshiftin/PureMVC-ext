<?php
class MyFacade extends Facade {
	private static $shouldCallParentAndEcho = false;

	static public function getInstance() {
		if(is_null(self::$instance));
			self::$instance = new self();
		return self::$instance;
	}

	public static function enableCallPAndE() {
		self::$shouldCallParentAndEcho = true;
	}

	protected function initializeFacade() {
		if(self::$shouldCallParentAndEcho) {
			parent::initializeFacade();
			echo 'Facade' . PHP_EOL;
		} else {
			$this->initializeModel();
			$this->initializeController();
			$this->initializeView();
		}
	}

	protected function initializeModel() {
		if(self::$shouldCallParentAndEcho) {
			parent::initializeModel();
			echo 'Model' . PHP_EOL;
		}
		if(is_null($this->model))
			$this->model= MyModel::getInstance();
	}

	protected function initializeView() {
		if(self::$shouldCallParentAndEcho) {
			parent::initializeView();
			echo 'View' . PHP_EOL;
		}
		if(is_null($this->view))
			$this->view = MyView::getInstance();
	}

	protected function initializeController() {
		if(self::$shouldCallParentAndEcho) {
			parent::initializeController();
			echo 'Controller' . PHP_EOL;
		}
		if(is_null($this->controller))
			$this->controller = MyController::getInstance();
	}

	public function notifyObservers($notification) {
		var_dump($notification);
	}
}
