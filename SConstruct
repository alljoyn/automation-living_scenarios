# Copyright AllSeen Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for any
#    purpose with or without fee is hereby granted, provided that the above
#    copyright notice and this permission notice appear in all copies.
#
#    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

import os

env = SConscript('../core/alljoyn/build_core/SConscript')

vars = Variables()
vars.Add('BINDINGS', 'Bindings to build (comma separated list): cpp', 'cpp,c,java') #Only cpp is working for now
vars.Add(PathVariable('ALLJOYN_DISTDIR',
                      'Directory containing a built AllJoyn Core dist directory.',
                      os.environ.get('ALLJOYN_DISTDIR')))
#vars.Add(EnumVariable('BUILD_SAMPLES',
#                      'Build the services samples.',
#                      'on',
#                      allowed_values = ['on', 'off']))
#vars.Add(EnumVariable('BUILD_TESTS',
#                      'Build the services samples.',
#                      'on',
#                      allowed_values = ['on', 'off']))
vars.Update(env)
Help(vars.GenerateHelpText(env))

#To see later, for using already builded core and/or DDAPI lib and inc

#if env.get('ALLJOYN_DISTDIR'):
    # normalize ALLJOYN_DISTDIR first
#    env['ALLJOYN_DISTDIR'] = env.Dir('$ALLJOYN_DISTDIR')
#    env.Append(CPPPATH = [ env.Dir('$ALLJOYN_DISTDIR/cpp/inc'),
#                           env.Dir('$ALLJOYN_DISTDIR/about/inc') ])
#    env.Append(LIBPATH = [ env.Dir('$ALLJOYN_DISTDIR/cpp/lib'),
#                           env.Dir('$ALLJOYN_DISTDIR/about/lib') ])

bindings = set([ b.strip() for b in env['BINDINGS'].split(',') ])
env['bindings'] = bindings

# Always build Datadriven_api if the binding is in cpp
if 'cpp' in bindings :
	if os.path.exists('../data/datadriven_api/SConscript'):
		env['BUILD_SAMPLES'] = 'off' #To be removed once the samples are wrote
		env['BUILD_TESTS'] = 'off' #To be removed once the unit tests are wrote
		env.SConscript(['../data/datadriven_api/SConscript'])

env.SConscript('SConscript')
