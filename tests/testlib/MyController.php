<?php
class MyController extends Controller {
	private static $shouldSuppressEcho = false;
	private static $amWacky = false;
	private static $shouldUseMyView = false;

	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public static function disableEcho() {
		self::$shouldSuppressEcho  = true;
	}

	public static function enableWacky() {
		self::$amWacky = true;
	}

	public static function enableMyView() {
		self::$shouldUseMyView = true;
	}

	public function hasCommand($notificationName) {
		if(!self::$shouldSuppressEcho)
			echo $notificationName . PHP_EOL;
		return parent::hasCommand($notificationName);
	}

	public function registerCommand($notificationName, $commandClassRef) {
		if(!self::$shouldSuppressEcho)
			echo "$notificationName $commandClassRef" . PHP_EOL;
		parent::registerCommand($notificationName, $commandClassRef);
	}

	public function removeCommand($notificationName) {
		if(!self::$shouldSuppressEcho)
			echo $notificationName . PHP_EOL;
	}

	public function initializeController() {
		if(self::$amWacky) {
			parent::initializeController();
			var_dump($this->view);
			var_dump($this->commandMap);
		} elseif(self::$shouldUseMyView) {
			$this->view = MyView::getInstance();
		}
	}

	public function dumpCommandMap() {
		var_dump($this->commandMap);
	}
}
