<?php
class MyController extends Controller {
	private static $shouldSuppressEcho = false;
	private static $amWacky = false;

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

	public function hasCommand($notificationName) {
		if(!self::$shouldSuppressEcho)
			echo $notificationName . PHP_EOL;
	}

	public function registerCommand($notificationName, $commandClassRef) {
		if(!self::$shouldSuppressEcho)
			echo "$notificationName $commandClassRef" . PHP_EOL;
	}

	public function removeCommand($notificationName) {
		if(!self::$shouldSuppressEcho)
			echo $notificationName . PHP_EOL;
	}

	public function executeCommand($notification) {
		var_dump($notification);
		parent::executeCommand($notification);
	}

	public function initializeController() {
		if(self::$amWacky) {
			parent::initializeController();
			var_dump($this->view);
			var_dump($this->commandMap);
		}
	}
}
