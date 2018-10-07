#!/usr/bin/env python
# This script will download and extract required tools into the current directory.
# Tools list is obtained from package/package_esp8266com_index.template.json file.
# Written by Ivan Grokhotkov, 2015.
#
from __future__ import print_function
import os
import shutil
import errno
import os.path
import hashlib
import json
import platform
import sys
import tarfile
import zipfile
import re
if sys.version_info[0] == 3:
    from urllib.request import urlretrieve
else:
    # Not Python 3 - today, it is most likely to be Python 2
    from urllib import urlretrieve

dist_dir = 'dist/'

def sha256sum(filename, blocksize=65536):
    hash = hashlib.sha256()
    with open(filename, "rb") as f:
        for block in iter(lambda: f.read(blocksize), b""):
            hash.update(block)
    return hash.hexdigest()

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno != errno.EEXIST or not os.path.isdir(path):
            raise

def report_progress(count, blockSize, totalSize):
    percent = int(count*blockSize*100/totalSize)
    percent = min(100, percent)
    sys.stdout.write("\r%d%%" % percent)
    sys.stdout.flush()

def unpack(filename, destination, rename_to):
    dirname = ''
    dirpath = destination
    print('Extracting {0}'.format(filename))
    if filename.endswith('tar.gz'):
        tfile = tarfile.open(filename, 'r:gz')
        dirname= tfile.getnames()[0]
        file_names = tfile.getnames()
        for file_name in file_names:
            if file_name.find(dirname) < 0:
                dirname = rename_to
                dirpath = os.path.join(destination, rename_to)
                break;
        tfile.extractall(dirpath);
    elif filename.endswith('zip'):
        zfile = zipfile.ZipFile(filename)
        dirname = zfile.namelist()[0]
        file_names = tfile.namelist()
        for file_name in file_names:
            if file_name.find(dirname) < 0:
                dirname = rename_to
                dirpath = os.path.join(destination, rename_to)
                break;
        zfile.extractall(dirpath)
    else:
        raise NotImplementedError('Unsupported archive type')

    if rename_to != dirname:
        print('Renaming {0} to {1}'.format(dirname, rename_to))
        if os.path.isdir(rename_to):
            shutil.rmtree(rename_to)
        shutil.move(dirname, rename_to)

def get_tool(tool):
    archive_name = tool['archiveFileName']
    local_path = dist_dir + archive_name
    url = tool['url']
    real_hash = tool['checksum'].split(':')[1]
    if not os.path.isfile(local_path):
        print('Downloading ' + archive_name);
        urlretrieve(url, local_path, report_progress)
        sys.stdout.write("\rDone\n")
        sys.stdout.flush()
    else:
        print('Tool {0} already downloaded'.format(archive_name))
    local_hash = sha256sum(local_path)
    if local_hash != real_hash:
        print('Hash mismatch for {0}, delete the file and try again'.format(local_path))
        raise RuntimeError()
    unpack(local_path, '.', tool['name'])

def load_tools_list(filename, platform):
    tools_info = json.load(open(filename))['packages'][0]['tools']
    tools_to_download = []
    for t in tools_info:
        tool_platform = [p for p in t['systems'] if p['host'] == platform]
        if len(tool_platform) == 0:
            continue
        tool_platform[0]['name'] = t['name']
        tools_to_download.append(tool_platform[0])
    return tools_to_download

def identify_platform():
    arduino_platform_names = {'Darwin'  : {32 : 'darwin_x86_64', 64 : 'darwin_x86_64'},
                              'Linux'   : {32 : 'linux_i686', 64 : 'linux_x86_64'},
                              'Windows' : {32 : 'windows_i686', 64 : 'windows_i686'}}
    bits = 32
    if sys.maxsize > 2**32:
        bits = 64
    sys_name = platform.system()
    if 'Linux' in sys_name and (platform.platform().find('arm') > 0 or platform.platform().find('aarch64') > 0):
        sys_name = 'LinuxARM'
    if 'CYGWIN_NT' in sys_name:
        sys_name = 'Windows'
    return arduino_platform_names[sys_name][bits]

if __name__ == '__main__':
    print('Platform: {0}'.format(identify_platform()))
    tools_to_download = load_tools_list('./package_index.json', identify_platform())
    mkdir_p(dist_dir)
    for tool in tools_to_download:
        get_tool(tool)
