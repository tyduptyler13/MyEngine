{
	'targets': [
		{
			'target_name': 'MyEngineNode',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [ 'src/binding.cpp' ],
			'dependencies': [ 'build.gyp:MyEngine' ],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ]
		}
	]

}