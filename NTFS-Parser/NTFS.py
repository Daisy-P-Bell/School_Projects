#arguments: /media/daisy/MSHIP_042/NTFS/ntfs-test.img 2048
import sys
from struct import *
from datetime import *

NTFS_BOOT_SECTOR = 512
NTFS_MAGIC_STRING = 'NTFS'
FILE_MAGIC_STRING = 'FILE'
MFT_MASTER_RECORD_SIZE = 1024
FILE_SIGNATURE_MAX_LENGTH = 8
CLUSTER_SIZE = 4096
MFT_OFFSET = 1064960
STANDARD_ATTRIBUTE_OFFSET = 56

print("test: %s" % (sys.argv[1]))
IMAGE = sys.argv[1]
VOLOFFSET = sys.argv[2]

print("IMAGE: %s" % IMAGE)
print("VOLOFFSET: %s" % VOLOFFSET)


#24
STANDARD_ATTRIBUTE_HEADER_RESIDENT_NONAME = ('<'
     # | Offset | Size | Value | Description                      |
     # |--------|------|-------|----------------------------------|
'I'  # | 0      | 4    |       | Attribute Type (e.g. 0x10, 0x60) |
'I'  # | 4      | 4    |       | Length (including this header)   |
'B'  # | 8      | 1    | 0     | Non-resident flag                |
'B'  # | 9      | 1    | 0     | Name length                      |
'H'  # | A      | 2    | 0     | Offset to the Name               |
'H'  # | C      | 2    | 0     | Flags                            |
'H'  # | E      | 2    |       | Attribute Id (a)                 |
'I'  # | 10     | 4    | L     | Length of the Attribute          |
'H'  # | 14     | 2    | 18    | Offset to the Attribute          |
'B'  # | 16     | 1    |       | Indexed flag                     |
'x') # | 17     | 1    | 0     | Padding                          |
     # | 18     | L    |       | The Attribute                    |

STANDARD_ATTRIBUTE_HEADER_RESIDENT_NAMED = ('<'
     # | Offset | Size | Value | Description                      |
     # |--------|------|-------|----------------------------------|
'I'  # | 0      | 4    |       | Attribute Type (e.g. 0x90, 0xB0) |
'I'  # | 4      | 4    |       | Length (including this header)   |
'B'  # | 8      | 1    | 0     | Non-resident flag                |
'B'  # | 9      | 1    | N     | Name length                      |
'H'  # | A      | 2    | 0x18  | Offset to the Name               |
'H'  # | C      | 2    | 0     | Flags                            |
'H'  # | E      | 2    |       | Attribute Id (a)                 |
'I'  # | 10     | 4    | L     | Length of the Attribute          |
'H'  # | 14     | 2    |2N+0x18| Offset to the Attribute (b)      |
'B'  # | 16     | 1    |       | Indexed flag                     |
'x') # | 17     | 1    | 0     | Padding                          |
     # | 18     | 2N   |Unicode| The Attribute's name             |
     # | 2N+0x18| L    |       | The Attribute  (b)               |

STANDARD_ATTRIBUTE_HEADER_NONRESIDENT_NONAME = ('<'
     # | Offset | Size | Value | Description                      |
     # |--------|------|-------|----------------------------------|
'I'  # | 0      | 4    |       | Attribute Type (e.g. 0x20, 0x80) |
'I'  # | 4      | 4    |       | Length (including this header)   |
'B'  # | 8      | 1    | 0x01  | Non-resident flag                |
'B'  # | 9      | 1    | 0     | Name length                      |
'H'  # | A      | 2    | 0     | Offset to the Name               |
'H'  # | C      | 2    |       | Flags                            |
'H'  # | E      | 2    |       | Attribute Id (a)                 |
'Q'  # | 10     | 8    |       | Starting VCN                     |
'Q'  # | 18     | 8    |       | Last VCN                         |
'H'  # | 20     | 2    | 0x40  | Offset to the Data Runs          |
'H'  # | 22     | 2    |       | Compression Unit Size (b)        |
'I'  # | 24     | 4    | 0     | Padding                          |
'Q'  # | 28     | 8    |       | Allocated size of the attribute (c) |
'Q'  # | 30     | 8    |       | Real size of the attribute       |
'Q') # | 38     | 8    |       | Initialized data size of the stream (d) |
     # | 40     | ...  |       | Data Runs                        |

#64
STANDARD_ATTRIBUTE_HEADER_NONRESIDENT_NAMED = ('<'
     # | Offset | Size | Value | Description                      |
     # |--------|------|-------|----------------------------------|
'I'  # | 0      | 4    |       | Attribute Type (e.g. 0x80, 0xA0) |
'I'  # | 4      | 4    |       | Length (including this header)   |
'B'  # | 8      | 1    | 1     | Non-resident flag                |
'B'  # | 9      | 1    | N     | Name length                      |
'H'  # | A      | 2    | 0x40  | Offset to the Name               |
'H'  # | C      | 2    |       | Flags                            |
'H'  # | E      | 2    |       | Attribute Id (a)                 |
'Q'  # | 10     | 8    |       | Starting VCN                     |
'Q'  # | 18     | 8    |       | Last VCN                         |
'H'  # | 20     | 2    | 2N+40 | Offset to the Data Runs(b)       |
'H'  # | 22     | 2    |       | Compression Unit Size (c)        |
'I'  # | 24     | 4    | 0     | Padding                          |
'Q'  # | 28     | 8    |       | Allocated size of the attribute (d) |
'Q'  # | 30     | 8    |       | Real size of the attribute       |
'Q') # | 38     | 8    |       | Initialized data size of the stream (e) |
     # | 40     | 2N   |Unicode| The Attribute's Name             |
     # | 2N+0x40| L    |       | Data Runs                        |



FILE_NAME_ATTRIBUTE_HEADER = ('<'
     # | Offset | Size | Value | Description                      |
     # |--------|------|-------|----------------------------------|
'Q'  # | 0      | 8    |       | File reference to the parent directory. |
'Q'  # | 8      | 8    |       | C Time - File Creation           |
'Q'  # | 10     | 8    | 1     | A Time - File Altered            |
'Q'  # | 18     | 8    | N     | M Time - MFT Changed             |
'Q'  # | 20     | 8    | 0x40  | R Time - File Read               |
'Q'  # | 28     | 8    |       | Allocated size of the file       |
'Q'  # | 30     | 8    |       | Real size of the file            |
'I'  # | 38     | 4    |       | Flags, e.g. Directory, compressed, hidden |
'I'  # | 3c     | 4    |       | Used by EAs and Reparse          |
'B'  # | 40     | 1    | 2N+40 | Filename length in characters (L)|
'B') # | 41     | 1    |       | Filename namespace               |
     # | 42     | 2L   | 0     | File name in Unicode (not null terminated) |


ifile = open(IMAGE, 'rb')
current_offset = MFT_OFFSET
file_record_string = True
i = 27
#i=67
while True:

    seek_offset = (MFT_OFFSET + (1024 * i))
    ifile.seek(seek_offset, 0)
    i+=1
    file_record = ifile.read(1024)
    magic_string = unpack_from(STANDARD_ATTRIBUTE_HEADER_RESIDENT_NONAME, file_record, 0)
    if magic_string[0] != 1162627398:
        break
    print("----------------------------------------------------------------------------")
    starting_values = unpack_from(STANDARD_ATTRIBUTE_HEADER_RESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET)
    if starting_values[0] == 4294967295:
        continue
    non_resident_flag = starting_values[2]
    name_length = starting_values[3]
    standard_attribute_length = starting_values[1]
    pre_file_name_data = unpack_from(STANDARD_ATTRIBUTE_HEADER_RESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length)
    file_name_data = unpack_from(FILE_NAME_ATTRIBUTE_HEADER, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length+24)
    #print("Standard Information Headers:", starting_values)
    #print("File Name Attribute Headers (pre file name data)", pre_file_name_data)
    #print("$FILE_NAME Data:", file_name_data)
    #print("")
    name_offset = seek_offset + STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + 24+66
    ifile.seek(name_offset, 0)
    name_record = ifile.read(2 * file_name_data[9])


    parent_directory = file_name_data[0]
    #print("parent_directory:", parent_directory)

    try:
        print("File Name:", name_record.decode('ascii'))
    except:
        print("CANNOT DISPLAY NAME")


    #PLEASE NOTE THAT THE FILETIME STUFF BELOW WAS TAKEN STRAIGHT FROM STACKOVERFLOW FROM HERE:
    # https://stackoverflow.com/questions/38878647/python-convert-filetime-to-datetime-for-dates-before-1970

    file_creation_date = file_name_data[1]
    #print("File Creation:", file_creation_date)
    us = (file_creation_date - 116444736000000000) // 10
    print("File Creation:", datetime(1970, 1, 1) + timedelta(microseconds = us))

    file_altered_date = file_name_data[2]
    #print("File Last Altered:", file_altered_date)
    us = (file_altered_date - 116444736000000000) // 10
    print("File Last Altered:", datetime(1970, 1, 1) + timedelta(microseconds=us))

    allocated_file_size = file_name_data[5]
    print("Allocated File Size:", allocated_file_size)
    if allocated_file_size == 0:
        print("Actual File Size: 0")  #This means it's a directory
        print("File Type: Directory")

    parent_directory_hex = file_name_data[0]
    (directory_test, ) = unpack_from('I', file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length+24)
    #directory_test += 1
    path_name = name_record.decode('ascii')
    if directory_test < 27:
        print("Path:", '/' + path_name)
    else:
        while directory_test > 27 :
            parent_offset = (directory_test * 1024) + MFT_OFFSET
            #print("Parent Offset:", parent_offset)
            ifile.seek(parent_offset ,0)
            parent_file_record = ifile.read(1024)
            #print('Directory Test', directory_test)
            seeek_offset = (MFT_OFFSET + (1024 * directory_test))
            (directory_test, ) = unpack_from('I', parent_file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length+24)
            filee_name_data = unpack_from(FILE_NAME_ATTRIBUTE_HEADER, parent_file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length+24)
            namee_offset = seeek_offset + STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + 24 + 66
            ifile.seek(namee_offset, 0)
            namee_record = ifile.read(2 * filee_name_data[9])
            name = namee_record.decode('ascii')

            path_name = name + '/' + path_name
        print("PATH:", path_name)




    if file_name_data[6] != 0:
        actual_file_size = file_name_data[6]
        print("Actual File Size:", actual_file_size)
    fourth_header = unpack_from(STANDARD_ATTRIBUTE_HEADER_NONRESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + pre_file_name_data[1])
    third_header = unpack_from(STANDARD_ATTRIBUTE_HEADER_NONRESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + pre_file_name_data[1])

    data_run_header_offset = STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + pre_file_name_data[1]


    while third_header[0] != 128:
        if third_header[0] > 128:
            break
        #print("THIRD HEADER [0]:", third_header[0])
        data_run_header_offset = STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length + pre_file_name_data[1] + third_header[1]
        #print("THIRDHEADER[1]", third_header[1])
        fourth_header =unpack_from(STANDARD_ATTRIBUTE_HEADER_RESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length  + pre_file_name_data[1] + third_header[1])
        third_header = unpack_from(STANDARD_ATTRIBUTE_HEADER_NONRESIDENT_NONAME, file_record, STANDARD_ATTRIBUTE_OFFSET + standard_attribute_length  + pre_file_name_data[1] + third_header[1])

    #print("Third Header:", third_header)
    if third_header[2] == 1 and third_header[3] == 0:  #NON-RESIDENT NONAME
        if file_name_data[6] == 0:
            #print("Final Third Header:", third_header)
            actual_file_size = third_header[13]
            print("Actual File Size:", actual_file_size)

        offset_to_the_data_runs = third_header[9]
        allocated_size_of_the_attribute = third_header[12]
        real_size_of_the_attribute = third_header[13]

        (run_header,) = unpack_from('1s', file_record, data_run_header_offset + offset_to_the_data_runs)
        run_header = int.from_bytes(run_header, 'little')
        run_length_length = run_header % 16
        run_lcn_length = (run_header // 16) % 16

        format_string = ''+str(run_length_length)+'s'+str(run_lcn_length)+'s'
        (length, offset) = unpack_from(format_string, file_record, data_run_header_offset + offset_to_the_data_runs + 1)

        run_length = int.from_bytes(length, 'little')
        run_lcn = int.from_bytes(offset, 'little')
        #print("offset:", offset)
        #print("run length:", run_length)
        #print("run_lcn", run_lcn)
        data_run_begin = 2048 + ((run_lcn+255) * 4096) +2048
        #print("DATA_RUN_BEGIN", data_run_begin)
        ifile.seek(data_run_begin, 0)
        file_magic = ifile.read(FILE_SIGNATURE_MAX_LENGTH)

        if int.from_bytes(file_magic, "little") == 15879423200937967616 :
            file_magic = "txt.swp"
        elif int.from_bytes(file_magic, "little") == 3687939215385186341 :
            file_magic = "PDF"
        elif int.from_bytes(file_magic, "little") == 1126195085246464 :
            file_magic = "mpg"
        elif int.from_bytes(file_magic, "little") == 2314885530448103949 :
            file_magic = "txt"
        elif int.from_bytes(file_magic, "little") == 2314885530818453536 :
            file_magic = "txt"
        elif int.from_bytes(file_magic, "little") == 727905341920923785 :
            file_magic = "png"
        elif int.from_bytes(file_magic, "little") == 5064878326892452095 :
            file_magic = "jpg"
        elif int.from_bytes(file_magic, "little") == 7510946913960880188 :
            file_magic = "html"
        elif int.from_bytes(file_magic, "little") == 168991884411554119 :
            file_magic = "gif"
        elif int.from_bytes(file_magic, "little") == 8595200589 :
            file_magic = "exe"
        elif int.from_bytes(file_magic, "little") == 1686174719 :
            file_magic = "mp3"
        print("File Type:", file_magic)


    elif third_header[2] == 0 and third_header[3] == 0:
        length_of_the_attribute = fourth_header[7]
        offset_to_the_attribute = fourth_header[8]

        read_how_many = FILE_SIGNATURE_MAX_LENGTH
        if length_of_the_attribute < FILE_SIGNATURE_MAX_LENGTH:
            read_how_many = length_of_the_attribute
        (signature,) = unpack_from( str(read_how_many) + 's', file_record, data_run_header_offset + offset_to_the_attribute)
        if int.from_bytes(signature, "little") == 7510946913960880188:
            print("File Type: HTML")

    if name_length != 0 and non_resident_flag != 0:
        print("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH")
    print("i:", i)
    print("----------------------------------------------------------------------------")
    if starting_values == (0, 0, 0, 0, 0, 0, 0, 0, 0, 0):
        print("end")


ifile.close()
