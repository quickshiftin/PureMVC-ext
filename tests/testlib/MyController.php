<?php
class MyController extends Controller {
	public static function getInstance() {
		if(is_null(self::$instance))
			self::$instance = new self();
		return self::$instance;
	}

	public function hasCommand($notificationName) {
		echo $notificationName . PHP_EOL;
	}

	public function registerCommand($notificationName, $commandClassRef) {
		echo "$notificationName $commandClassRef" . PHP_EOL;
	}

	public function removeCommand($notificationName) {
		echo $notificationName . PHP_EOL;
	}
}
