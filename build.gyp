{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "executable",
			"sources": [
				"<!@(ls src/*)"
			],
			"include_dirs": [
				"include"
			],
			"direct_dependent_settings": {
				"include_dirs": [ "include/" ]
			},
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++11"
			]
		}
	]
}

