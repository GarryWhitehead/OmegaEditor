#include "OmegaEngineEditor.h"
#include <QtWidgets/QApplication>
#include "utility/file_log.h"
#include "EntityManager.h"
#include <qtimer.h>
#include <qobject.h>


int main(int argc, char *argv[])
{
	//Initialise file logging for debugging purposes
	g_filelog = new FileLog("editor-debug", (uint32_t)FileLogFlags::FILELOG_WRITE_DATE_INIT);

	// init the entity manager which deals with entity creation and serialisation of data
	entityManager = new EntityManager();

	QApplication editorApp(argc, argv);
	MainWindow editor;

	editor.show();
	return editorApp.exec();
}
