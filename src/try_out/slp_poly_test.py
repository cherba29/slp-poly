# Copyright 2016 Arthur D. Cherba
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import contextlib
import os
import re
import shutil
import subprocess
import tempfile
import unittest
import yaml

_APP_NAME = './slp-poly'


@contextlib.contextmanager
def TempDir():
  temp_dir = tempfile.mkdtemp()
  try:
    yield temp_dir
  finally:
    shutil.rmtree(temp_dir)


@contextlib.contextmanager
def TempFileName(filename):
  with TempDir() as temp_dir:
    yield os.path.join(temp_dir, filename)


class SlpPolyTest(unittest.TestCase):

  def setUp(self):
    self.maxDiff = None

  def assertMatches(self, text, pattern, msg=None):
    if not re.match(pattern, text, re.MULTILINE):
      raise AssertionError(
          "String text does not match regexp pattern\n"\
          "    text:    %r\n"\
          "    pattern: %r" % (text, pattern))

  def TmpFileName(self, filename):
    tmp_dir = os.environ.get('TEST_TMPDIR')
    if tmp_dir is None:
      raise AssertionError('TEST_TMPDIR env variable is not set. '
                           'Environment {}'
                           .format(yaml.dump(os.environ,
                                             default_flow_style=False)))
    return os.path.join(tmp_dir, filename)

  def Run(self, *args):
    cmd = (_APP_NAME, ) + args
    process = subprocess.Popen(
        cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return process.returncode, stdout, stderr

  def testNoArguments(self):
    returncode, stdout, stderr = self.Run()
    self.assertEqual(1, returncode)
    self.assertMatches(
        stdout,
        '^slp-poly - Polynomial Interpolator \(slp-poly\) Version ' 
        '[0-9]\.[0-9]\.[0-9] Commit [a-z0-9]\n\n$')
    self.assertMultiLineEqual(
        '\n'.join([
            'ERROR At least one of the options',
            '  version, ',
            '  benchmark, ',
            '  machine, ',
            '  interpolate',
            'must be specified on a command line. '
            'Use -h for help on possible options.',
            '',
            ''
        ]),
        stderr)

  def testHelp(self):
    returncode, stdout, stderr = self.Run('--help')
    self.assertEqual(2, returncode, stderr)
    self.assertMatches(
        stdout,
        '^slp-poly - Polynomial Interpolator \(slp-poly\) Version ' 
        '[0-9]\.[0-9]\.[0-9] Commit [a-z0-9]\n\n$')
    self.assertMultiLineEqual("""\
Allowed options:

Main (mutually exclusive) Options:
  -h [ --help ]             produce this help message. Use in conjunction with 
                            other options to get more help, e.g. -h -t will 
                            show interpolation options
  -v [ --version ]          print version string
  -m [ --machine ]          Output machine information into given filename.
  -b [ --benchmark ]        Only run benchmarks and save into given filename.
  -t [ --interpolate ]      Interpolate problem given in the input file, saving
                            in the given output file. run -ht option to see 
                            more info.

General Options:
  -l [ --level ] arg        <module name>=<log level>. Log levels:
                              trace - Very detailed program execution trace. 
                                      Includes all other level.
                              debug - Less detailed than trace, used by key 
                                      complex areas of code.
                              info  - (default) Application level/domain 
                                      specific messages.
                              inf1  - More detailed application messages, shows
                                      all info as well.
                              inf2  - Even more detailed application messages, 
                                      shows all info and info1.
                              warning - Warnings.
                              error -  Only show severe problems.
                              fatal -  Fatal errors.
                              none - No tracing not even errors.
  --log-tags arg (=itml)    Show various tags in the log
                            i - Incrementing index (first default)
                            t - Running timer (second default)
                            m - Module name (third default)
                            l - Log Level name (see level option)
                            n - File name and line number
                            f - C++ Function name (too verbose)
                            h - Thread id
  --response-file arg       Config file with same syntax as command line. Can 
                            be specified with '@name', too.
  -o [ --output-file ] arg  Output file to save results in yaml format.
                            If not specified the output will be dumped tothe 
                            terminal at the end of execution
  -w [ --overwrite ]        overwrite output file if it exists

""",
        stderr)

  def testVersionFlag(self):
    returncode, stdout, stderr = self.Run('--version')
    self.assertEqual(0, returncode)
    self.assertMatches(
        stdout,
        '^slp-poly - Polynomial Interpolator \(slp-poly\) Version ' 
        '[0-9]\.[0-9]\.[0-9] Commit [a-z0-9]\n\n$')
    self.assertMatches(
        stderr,
        '^  Version [0-9]\.[0-9]\.[0-9] built ' 
        '[0-9]{4}-[A-z][a-z]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2} '
        'Revision 0 Committed on \n'
        '    Compiled with GCC compiler version [^\n]+\n'
        '  Report bugs, suggestions or comments to https://github.com/cherba29/slp-poly\n'
        '\n$')

  def testBenchmarkHelpFlag(self):
    returncode, stdout, stderr = self.Run('-b', '--help')
    self.assertEqual(2, returncode)
    self.assertMatches(
        stdout,
        '^slp-poly - Polynomial Interpolator \(slp-poly\) Version ' 
        '[0-9]\.[0-9]\.[0-9] Commit [a-z0-9]\n\n$')
    self.assertMatches(
        stderr,
        '^\nUsage: ./slp-poly -b \[-o <output file>\]\n'
        '\n'
        'Measure speed of key algorithms on this machine,\n'
        'saving output in YAML format output file\n$')

  def testBenchmarkOutputFile(self):
    with TempFileName('benchmark.yaml') as output_file:
      returncode, stdout, stderr = self.Run('-b', '-o', output_file)
      self.assertEqual(0, returncode)
      with open(output_file) as f:
        content = f.read()

    self.assertMatches(
        stdout,
        '^slp-poly - Polynomial Interpolator \(slp-poly\) Version ' 
        '[0-9]\.[0-9]\.[0-9] Commit [a-z0-9]\n\n$')
    try:
      data = yaml.load(content)
    except Exception as e:
      self.fail('Failed to parse\n{}\n\n{}'.format(content, e))
    self.assertEqual(output_file, data['output']['filename'])
    self.assertEqual('slp-poly', data['tool']['name'])
    self.assertEqual('Success', data['execution']['status'])
    self.assertIn('Field_uint32_3221225473', data['benchmark']['field'])
    self.assertIsNotNone(
        data['benchmark']['field']['Field_uint32_3221225473']
        ['vandermonde']['setup']['quad']['equation'])

if __name__ == '__main__':
  unittest.main()
