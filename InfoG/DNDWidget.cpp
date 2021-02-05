#include "DNDWidget.h"

#include "DNDFileManager.h"

DNDWidget::DNDWidget() {

	notified = false;
	DNDFileManager::subscribe(this);

}

void DNDWidget::notify() {
	notified = true;
}