# Реализация техники Inject&Hook

## Компиляция проекта
Решение успешно компилируется в Microsoft Visual Studio 2019 в Release и в Debug версии x64. Также можно скомпилировать в командной строке разработчика от Visual Studio 2019
с помощью команд devenv LabSec.sln /build Debug и devenv LabSec.sln /build Release.
## Запуск программы
После успешной компиляции в директории bin\Release или bin\Debug должны появиться файлы Inject.exe и Hook.dll. Для успешного выполнения программы необходимо в начале убедиться, что процесс, в котором необходимо произвести инжекцию запущен. После чего, необходимо запустить cmd от имени администратора и в указанной выше директории запустить Inject.exe с необходимыми параметрами.
## Формат входных параметров
Поддерживается следующий формат входных данных:
1. –pid < Target_Process_PID> – целевой процесс выбирается по значению PID.
2. –name <Target_Process_Name> – целевой процесс выбирается по имени.
3. –func <Function_Name> – имя функции, вызов которой требуется отследить.
4. –hide <File_Name> – имя файла, который надо скрыть от процесса.

Возможные сценарии запуска программы:
1. Inject.exe –pid 123 –func CreateFile
2.	Inject.exe –name notepad.exe –func CreateFileW
3.	Inject.exe –name notepad.exe –hide “C:\input.txt”

## Краткая документация
 В решении имеется 3 проекта Detours - содержит исходные файлы стороннего ПО Detours, Inject - содержит исходные файлы модуля, предназначенного для инжекции DLL в процесс, Hook - содержит исходные файлы модуля, реализущего DLL, которая будет отслеживать и осуществлять подмену Win API.
 1. Проект Hook содержит следующие исходный файлы:  
  * hook.h - содержит бибилиотеки, которые необходимо подключить для компиляции, а также объявление функций подмен.
  * hook.cpp - реализация DLL, которая будет инжектирована в процесс
  * myfunc.cpp - реализация функций помдмен
  * hookasm.asm - реализация функции подмены на языке ассемблер
2. Проект Inject содержит следуюшие исходные файлы:  
  * inject.h - содержит библиотеки, которые необходимо подкключить для компиляции.
  * inject.cpp - реализация модуля, отвечающего за инжектирование
  * mysocket.h - оъявление класса сокетов
  * mysocket.cpp - определение класса сокетов 
