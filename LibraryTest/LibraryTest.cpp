// ConsoleApplication2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <tchar.h>
#include <iostream>
#include "DataAccessLibConnect.h"
#include <string.h>
#include <windows.h>

using namespace std;

void ErrorCheck();
void StorageTest();
int main()
{
    if (DataAccessLibInit()) {
        cout << "Library opened successfully" << endl;
    }
    else {
        cout << "Error happend with opening library" << endl;
    }

    StorageTest();

    //DataAccessLibCleanup();

    return 0;
}
void ErrorCheck()
{
    if (HasError())
    {
        char ErrorString[1024]; // Статический массив для хранения ошибки
        GetLastErrorString(ErrorString);
        cout << "Reason: " << ErrorString << endl; // Вывод сообщения об ошибке
    }
}

void PrintStorageType(StorageType type) {
    switch (type) {
    case StorageType::LogicalDrive: wcout << L"LogicalDrive" << endl;   break;
    case StorageType::PhysicalDrive: wcout << L"PhysicalDrive" << endl; break;
    case StorageType::ImageFile: wcout << L"ImageFile" << endl;   break;
    case StorageType::DataStream: wcout << L"DataStream" << endl;    break;
    case StorageType::DirectoryFile: wcout << L"DirectoryFile" << endl;  break;
    case StorageType::RAIDx: wcout << L"RAIDx" << endl;    break;
    case StorageType::VMDK: wcout << L"VMDK" << endl;    break;
    case StorageType::VDI: wcout << L"VDI" << endl;    break;
    case StorageType::VHD: wcout << L"VHD" << endl;    break;
    case StorageType::VHDX: wcout << L"VHDX" << endl;  break;
    case StorageType::QEMU: wcout << L"QEMU" << endl;  break;
    case StorageType::HDD: wcout << L"HDD" << endl;  break;
    case StorageType::ErrorType: wcout << L"ErrorType" << endl;  break;
    case StorageType::UnknownType: wcout << L"UnknownType" << endl;   break;
    }
}

void PrintFileSystemType(FileSystemTypeEnum type) {
    switch (type) {
    case FileSystemTypeEnum::NTFS:        wcout << L"NTFS" << endl; break;
    case FileSystemTypeEnum::FAT12:       wcout << L"FAT12" << endl; break;
    case FileSystemTypeEnum::FAT16:       wcout << L"FAT16" << endl; break;
    case FileSystemTypeEnum::FAT32:       wcout << L"FAT32" << endl; break;
    case FileSystemTypeEnum::exFAT:       wcout << L"exFAT" << endl; break;
    case FileSystemTypeEnum::Ext2:        wcout << L"Ext2" << endl; break;
    case FileSystemTypeEnum::Ext3:        wcout << L"Ext3" << endl; break;
    case FileSystemTypeEnum::Ext4:        wcout << L"Ext4" << endl; break;
    case FileSystemTypeEnum::DFS:         wcout << L"DFS" << endl; break;
    case FileSystemTypeEnum::MBR:         wcout << L"MBR" << endl; break;
    case FileSystemTypeEnum::PMBR:        wcout << L"PMBR (защитный MBR для GPT)" << endl; break;
    case FileSystemTypeEnum::EMBR:        wcout << L"EMBR (расширенный MBR)" << endl; break;
    case FileSystemTypeEnum::GPT:         wcout << L"GPT" << endl; break;
    case FileSystemTypeEnum::GPT_4K:      wcout << L"GPT_4K" << endl; break;
    case FileSystemTypeEnum::PTFS:        wcout << L"PTFS (виртуальные файловые системы)" << endl; break;
    case FileSystemTypeEnum::FS_Custom:   wcout << L"Custom FS" << endl; break;
    case FileSystemTypeEnum::FS_None:     wcout << L"No FS" << endl; break;
    case FileSystemTypeEnum::Encrypted:   wcout << L"Encrypted" << endl; break;
    case FileSystemTypeEnum::FS_Error:    wcout << L"Error" << endl; break;
    case FileSystemTypeEnum::FS_Debug:    wcout << L"Debug" << endl; break;
    case FileSystemTypeEnum::VMDK:        wcout << L"VMDK" << endl; break;
    case FileSystemTypeEnum::VDI:         wcout << L"VDI" << endl; break;
    case FileSystemTypeEnum::VHD:         wcout << L"VHD" << endl; break;
    case FileSystemTypeEnum::VHDX:        wcout << L"VHDX" << endl; break;
    case FileSystemTypeEnum::QEMU:        wcout << L"QEMU" << endl; break;
    case FileSystemTypeEnum::HDD:         wcout << L"HDD (Parallels)" << endl; break;
    case FileSystemTypeEnum::TAR:         wcout << L"TAR" << endl; break;
    default:                              wcout << L"Unknown" << endl; break;
    }
}


void StorageTest()
{
    wstring physicalStoragePath(L"D:\\university\\ext4.img");
    StorageHandle storage = CreateStorageHandle(physicalStoragePath.c_str());
    if (storage != STORAGE_ERROR) {
        wcout << L"Descriptor created successfuly" << endl;

        StorageType storageType;
        storageType = GetStorageType(storage);
        wcout << L"StorageType: ";
        PrintStorageType(storageType);
        cout << endl;


        if (storageType == StorageType::PhysicalDrive) {
            PhysicalDriveInfoStruct info = GetPhysicalDriveInfo(storage);
            if (!info.Error) {
                wcout << L"Device: " << info.Device << endl;
                wcout << L"ProductId: " << info.ProductId << endl;
                wcout << L"Serial: " << info.Serial << endl;
                wcout << L"Size: " << info.Size << endl;
            }
            else {
                wcout << L"Error occured trying to get information about physical drive" << endl;
                ErrorCheck();
            }
        }
        cout << endl;

        int NumberOfPartitions = GetNumberOfPartitions(storage);
        wcout << L"Number of partitions: " << NumberOfPartitions << endl;
        cout << endl;

        PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;
        for (unsigned int i = 1; i <= NumberOfPartitions; i++) {
            GetPartitionInfoByNumber(storage, i, PartitionInfo);
            wcout << L"Partition number: " << i << endl;
            wcout << L"Recognized Type: ";
            PrintFileSystemType(PartitionInfo->RecognizedType);
            wcout << L"Raw Type: " << PartitionInfo->RawType << endl;
            wcout << L"First Sector: " << PartitionInfo->FirstSector << endl;
            wcout << L"Last Sector: " << PartitionInfo->LastSector << endl;
            wcout << L"Size in sectors: " << PartitionInfo->SizeInSectors << endl;
            wcout << L"Type Guid: " << PartitionInfo->TypeGuid << endl;
            wcout << L"PartGuid: " << PartitionInfo->PartGuid << endl;
            wcout << L"Flags: " << PartitionInfo->Flags << endl;
            cout << "Name: " << PartitionInfo->Name << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        }
        delete PartitionInfo;
        cout << endl;

        DWORD StorageBlockSize = GetStorageBlockSize(storage);
        if (StorageBlockSize != 0) {
            wcout << "StorageBlockSize: " << StorageBlockSize << endl;
            wcout << L"Enter new storage block size: " << endl;
            DWORD NewStorageBlockSize;
            cin >> NewStorageBlockSize;
            if (storageType == StorageType::PhysicalDrive || storageType == StorageType::LogicalDrive || storageType == StorageType::DataStream) {
                if (NewStorageBlockSize % StorageBlockSize == 0) {
                    if (SetStorageBlockSize(storage, NewStorageBlockSize)) {
                        wcout << L"Storage block size successfuly changed: " << NewStorageBlockSize << endl;
                    }
                    else {
                        wcout << L"Error setting new storage block size" << endl;
                        ErrorCheck();
                    }
                }
                else {
                    wcout << L"New storage block size must be a multiple of the original one" << endl;
                }
            }
            else {
                if (SetStorageBlockSize(storage, NewStorageBlockSize)) {
                    wcout << L"Storage block size successfuly changed: " << NewStorageBlockSize << endl;
                }
                else {
                    wcout << L"Error setting new storage block size" << endl;
                    ErrorCheck();
                }

            }

            StorageIteratorHandle it = GetStorageIterator(storage);
            if (it != STORAGE_ERROR) {
                int k = 0;
                for (StorageBlockFirst(it); !StorageBlockIsDone(it); StorageBlockNext(it)) {
                    k++;
                }
                wcout << L"The number of sectors covered: " << k << endl;
                CloseStorageIterator(it);
            }
            else {
                wcout << L"Error creating iterator" << endl;
                ErrorCheck();
            }

        }
        else {
            wcout << L"Error getting storage block size" << endl;
            ErrorCheck();
        }
        CloseStorageHandle(storage);
    }
    else {
        wcout << L"Error creating descriptor" << endl;
        ErrorCheck();
    }
    cout << endl;
    cout << endl;
    wstring imageStoragePath(L"C:\\Users\\17art\\ext2.img");
    StorageHandle imageStorage = CreateStorageHandle(imageStoragePath.c_str());
    if (imageStorage != STORAGE_ERROR)
    {
        wcout << L"Descriptor created successfuly" << endl;

        DWORD blockSize = 0x100000; // 1 МБ
        BYTE* blockBuffer = new BYTE[blockSize];
        if (GetStorageType(imageStorage) == StorageType::ImageFile)
        {
            if (!SetStorageBlockSize(imageStorage, blockSize))
            {
                wcout << L"Block size setting error!" << endl;
            }
        }

        // Последовательный обход блоков хранилища
        ULONGLONG totalReadSize = 0;
        StorageIteratorHandle storageIterator = GetStorageIterator(imageStorage);
        for (StorageBlockFirst(storageIterator); !StorageBlockIsDone(storageIterator); StorageBlockNext(storageIterator))
        {
            LONGLONG readSize = ReadStorageData(storageIterator, blockSize, blockBuffer);
            if (readSize == blockSize)
            {
                totalReadSize += readSize;
            }
            else
            {
                wcout << L"Error reading!" << endl;
            }
        }
        CloseStorageIterator(storageIterator);

        wcout << L"All read: " << totalReadSize << L" bytes" << endl;//Выводит размер выделенного пространства под диск в байтах

        delete[] blockBuffer;


        CloseStorageHandle(imageStorage);
    }

    else {
        wcout << L"Error creating descriptor" << endl;
        ErrorCheck();
    }
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
