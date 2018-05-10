# mbed SDK
# Copyright (c) 2011-2013 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from __future__ import print_function, division, absolute_import

from abc import ABCMeta, abstractmethod


class Notifier(object):
    """
    Notifiers send build system events to a front end or may implement a front
    end themselves, displaying warnings and errors for a user.

    This is different from a logger in a few ways:
     * The structure of the events are defined by this interface.
     * A "progress" level is included allowing signaling completion status to
       users.
     * It's tailored to providing events from a build system.

    The structure of a message is a dict with a 'type' key. The type key
    determines the remaining keys as follows:
      type       | description and remaining keys
      ---------- | ------------------------------
      info       | A simple message. The 'message' key contains the message
      debug      | Another simple message; this one is less useful when compiles
                 | are working. Again, the 'message' key contains the message
      progress   | A progress indicator, which may include progress as a
                 | percentage. The action key includes what action was taken to
                 | make this progress, the file key what file was used to make
                 | this progress, and the percent key, when present, indicates
                 | how far along the build is.
      tool_error | When a compile fails, this contains the entire output of the
                 | compiler.
      var        | Provides a key, in the 'key' key, and a value, in the 'value'
                 | key, for use in a UI. At the time of writing it's used to
                 | communicate the binary location to the online IDE.
    """

    __metaclass__ = ABCMeta

    @abstractmethod
    def notify(self, event):
        """
        Send the user a notification specified in the event.
        """
        raise NotImplemented

    def info(self, message):
        """
        Send the user a simple message.
        """
        self.notify({'type': 'info', 'message': message})

    def debug(self, message):
        """
        Send a debug message to the user.
        """
        if isinstance(message, list):
            message = ' '.join(message)
        self.notify({'type': 'debug', 'message': message})

    def cc_info(self, info=None):
        if info is not None:
            info['type'] = 'cc'
            self.notify(info)

    def cc_verbose(self, message, file=""):
        self.notify({
            'type': 'cc',
            'severity': 'verbose',
            'file': file,
            'message': message
        })

    def progress(self, action, file, percent=None):
        """
        Indicate compilation progress to a user.
        """
        msg = {'type': 'progress', 'action': action, 'file': file}
        if percent:
            msg['percent'] = percent
        self.notify(msg)

    def tool_error(self, message):
        """
        Communicate a full fatal error to a user.
        """
        self.notify({'type': 'tool_error', 'message': message})

    def var(self, key, value):
        """
        Update a UI with a key, value pair
        """
        self.notify({'type': 'var', 'key': key, 'val': value})
