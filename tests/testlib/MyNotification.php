<?php
class MyNotification implements INotification {
	public function getName() {
		return 'testNotification';
	}
	public function getType() {}
	public function getBody() {}
	public function setName($name) {}
	public function setBody($body) {}
	public function setType($type) {}
	public function toString() {}
}
