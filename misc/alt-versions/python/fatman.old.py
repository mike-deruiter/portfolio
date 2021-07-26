#!/usr/bin/python3

import io, sys, array

if len(sys.argv) == 1:
    print("Please supply a .dd file to parse.")
    exit(1)

def bytes_to_ascii(byte_str):
    #byte_str = byte_str.replace(b'\x00', b'')
    return byte_str.decode('ascii').rstrip()
    
def bytes_to_int(byte_str, order = 'little'):
    return int.from_bytes(byte_str, byteorder = order)

class Volume():
    EOF = [b'\xff\xff', b'\x00\x00']

    def __init__(self, fname):
        self.vol = open(fname, "rb")
        self.read_boot_sector()
        self.determine_start_sectors()
        
    def read_boot_sector(self):
        self.vol.seek(3)
        self.oem_id = bytes_to_ascii(self.vol.read(8))
        self.bytes_per_sect = bytes_to_int(self.vol.read(2))
        self.sect_per_clust = bytes_to_int(self.vol.read(1))
        self.reserved_sect = bytes_to_int(self.vol.read(2))
        self.num_fat = bytes_to_int(self.vol.read(1))
        self.poss_root_entries = bytes_to_int(self.vol.read(2))
        self.vol.seek(22)
        self.sect_per_fat = bytes_to_int(self.vol.read(2))
        self.vol.seek(43)
        self.volume_label = bytes_to_ascii(self.vol.read(11))
        self.fs_type = bytes_to_ascii(self.vol.read(8))
    
    def determine_start_sectors(self):
        # Determine Start Sectors of Each Section
        self.reserved_region_start = 0
        self.fat_region_start = (self.reserved_region_start +
                                 self.sectors_to_bytes(self.reserved_sect))
        self.root_dir_start = (self.fat_region_start + 
                               self.sectors_to_bytes(self.num_fat * 
                                                     self.sect_per_fat))
        self.data_region_start = (self.root_dir_start + 
                                 self.sectors_to_bytes(self.calc_root_size()))
    
    def calc_root_size(self):
        root_entries_size = (self.poss_root_entries * 32) // self.bytes_per_sect
        root_entries_mod = (self.poss_root_entries * 32) % self.bytes_per_sect
        if root_entries_mod > 0:
            adjust = 1
        else:
            adjust = 0
        return root_entries_size + adjust
    
    def sectors_to_bytes(self, sectors):
        return sectors * self.bytes_per_sect
    
    def return_file(self, i_clust, fsize, deleted = False):
        file = b''
        slack = b''
        fsize_sofar = 0
        
        while True:
            i_clust = bytes_to_int(i_clust, 'little')
            
            self.vol.seek(self.data_region_start + 
                          self.sectors_to_bytes((i_clust - 2) *
                                                self.sect_per_clust))
        
            if fsize >= self.sectors_to_bytes(self.sect_per_clust):
                fsize = fsize - self.sectors_to_bytes(self.sect_per_clust)
                bytes_to_read = self.sectors_to_bytes(self.sect_per_clust)
                slack_to_read = 0
            else:
                bytes_to_read = fsize
                slack_to_read = self.sectors_to_bytes(
                                self.sect_per_clust) - fsize
                deleted = False # Break deleted loop
                
            file = file + self.vol.read(bytes_to_read)
            slack = slack + self.vol.read(slack_to_read)
            
            if not deleted:              
                self.vol.seek(self.fat_region_start + (i_clust * 2))
            
                i_clust = self.vol.read(2)
            else:
                i_clust = int.to_bytes(i_clust + 1, 'little')
            
            #print('next cluster: {0}'.format(i_clust))
            
            if i_clust in self.EOF:
                break        
            
        
        return file, slack
    
    def print_info(self):
        print('Filesystem Type:       {0}'.format(self.fs_type))
        print('OEM ID:                {0}'.format(self.oem_id))
        print('Volume Label:          {0}'.format(self.volume_label))
        print('Bytes Per Sector:      {0}'.format(self.bytes_per_sect))
        print('Sectors Per Cluster:   {0}'.format(self.sect_per_clust))
        print('Reserved Sectors:      {0}'.format(self.reserved_sect))
        print('Number of FAT:         {0}'.format(self.num_fat))
        print('Possible Root Entries: {0}'.format(self.poss_root_entries))
        print('Sectors Per FAT:       {0}'.format(self.sect_per_fat))
        print('FAT Region Start:      {0}'.format(hex(self.fat_region_start)))
        print('Root Directory Start:  {0}'.format(hex(self.root_dir_start)))
        print('Data Region Start:     {0}'.format(hex(self.data_region_start)))
        
class DirectoryEntry():
    def __init__(self, byte_str):
        self.dir_entry = io.BytesIO(byte_str)
        
        self.dir_entry.seek(11)
        self.attr = self.dir_entry.read(1)
        self.dir_entry.seek(0)
        
        self.fname = self.dir_entry.read(8)
        fname_dissect = array.array('B', self.fname)
        if fname_dissect[0] == 0:
            raise Exception
        elif fname_dissect[0] == 229: # b'\xe5'
            self.deleted = True
            fname_dissect[0] = 126 # b'~'
        else:
            self.deleted = False
        self.fname = bytes_to_ascii(fname_dissect.tobytes())
        
        self.f_ext = bytes_to_ascii(self.dir_entry.read(3))
        self.dir_entry.seek(26)
        self.starting_clust = self.dir_entry.read(2)
        self.filesize = bytes_to_int(self.dir_entry.read(4))
        
        if self.f_ext != '':
            self.filename = '{0}.{1}'.format(self.fname, self.f_ext)
        else:
            self.filename = '{0}'.format(self.fname)
        
    def print_info(self):
        if d.attr == b'\x20':
            print('Filename:        {0}'.format(self.filename))
            print('Attribute Byte:  {0}'.format(hex(int.from_bytes(self.attr, 
                                                                   'little'))))
            print('File Size:       {0}'.format(self.filesize))
            print('Starting Cluster {0}'.format(
                  hex(bytes_to_int(self.starting_clust))))
try:        
    v = Volume(sys.argv[1])
    v.print_info()
    print()
except Exception:
    print("Invalid .dd file supplied.")
    exit(2)

deleted_files = []

offset = 0
more_to_read = True
while more_to_read:
    v.vol.seek(v.root_dir_start + offset)

    try:
        d = DirectoryEntry(v.vol.read(32))
    except Exception:
        more_to_read = False
        continue

    offset = offset + 32

    d.print_info()

    if d.attr == b'\x20':   
        print()
        
        rf, slack = v.return_file(d.starting_clust, d.filesize)
        of = open(d.filename, 'wb')
   
        if not d.deleted:
            print('Writing {0}...'.format(d.filename))
        else:
            print('Recovering {0}...'.format(d.filename))
            deleted_files.append(d.filename)
    
        of.write(rf)
        
        if not slack == b'':
            print('Writing Slack of {0}...'.format(d.filename))
            sf = open(d.filename + '.slack', 'wb')
            sf.write(slack)

        print('Success.')
        print()
        
if not deleted_files == []:
    print('Deleted Files')
    print('=============')

    for file in deleted_files:
        print(file)
