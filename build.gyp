{
	"targets": [
		{
			"target_name": "libMyEngine",
			"type": "static_library",
			"sources": [
				"src/Material.cpp",
				"src/Log.cpp",
				"src/Math.cpp",
				"src/ShaderUtil.cpp",
				"src/GLES2Shader.cpp",
				"src/GLES2Renderer.cpp",
				"src/GLES2MaterialLib.cpp",
				"src/GLES2RenderTarget.cpp",
				"src/Texture.cpp",
				"src/GeometryImporter.cpp",
				"src/GLFWManager.cpp"
			],
			"include_dirs": [
				"include",
				"deps/Simple-OpenGL-Image-Library/src",
				"deps/assimp/include"
				#glfw is expected to be installed on each system target
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include",
					"deps/Simple-OpenGL-Image-Library/src",
					"deps/assimp/include"
				]
			},
			"link_dependent_settings": {
				"dependencies": [
					"glsl-optimizer"
				]
			},
			"link_settings": {
				"libraries": [
					"../deps/Simple-OpenGL-Image-Library/libSOIL.a",
					"../deps/assimp/lib/libassimp.a",
					"-lGLESv2"
				]
			},
			"variables": {
				"asmjs%": "0"
			},
			"conditions": [
				['OS=="win"', {
					"link_settings": {
						"libraries": [
							"-lgdi32",
							"-lglfw3"
						]
					}
				}],
				['OS=="linux"', {
					"conditions": [
						['asmjs==1', {
							"link_settings": {
								"libraries": [
									"-s USE_GLFW=3",
									"-lglfw3",
									"../deps/assimp/lib/libzlibstatic.a"
								]
							}
						}, {
							"link_settings": {
								"libraries": [
									"<!(pkg-config glfw3 --static --libs-only-l)",
									"-pthread" #We might need pthread for gcc (because their std::threads are broken otherwise)
								]
							}
						}]
					]
				}]
			],
			"dependencies": [
				"glsl-optimizer"
			],
			"ldflags": [ "-L../lib" ],
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++14",
				"-fpic"
			],
			'cflags!': [
				'-fno-exceptions'
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x'
			]
		},
		# glsl-optimizer code is a dependency of the engines shader compiler.
		# The settings have been adapted from cmake
		{
			"target_name": "glsl-optimizer",
			"type": "static_library",
			"variables": {
				"root": "deps/glsl-optimizer"
			},
			"include_dirs": [
				"<(root)/include",
				"<(root)/src/mesa",
				"<(root)/src/mapi",
				"<(root)/src/glsl",
				"<(root)/src"
			],
			"sources": [
				"<(root)/src/glsl/glcpp/glcpp-lex.c",
				"<(root)/src/glsl/glcpp/glcpp-parse.c",
				"<(root)/src/glsl/glcpp/pp.c",
				"<(root)/src/util/hash_table.c",
				"<(root)/src/util/ralloc.c",
				"<(root)/src/mesa/program/prog_hash_table.c",
				"<(root)/src/mesa/program/symbol_table.c",
				"<(root)/src/mesa/main/imports.c",
				"<(root)/src/glsl/ast_array_index.cpp",
				"<(root)/src/glsl/ast_expr.cpp",
				"<(root)/src/glsl/ast_function.cpp",
				"<(root)/src/glsl/ast_to_hir.cpp",
				"<(root)/src/glsl/ast_type.cpp",
				"<(root)/src/glsl/builtin_functions.cpp",
				"<(root)/src/glsl/builtin_types.cpp",
				"<(root)/src/glsl/builtin_variables.cpp",
				"<(root)/src/glsl/glsl_lexer.cpp",
				"<(root)/src/glsl/glsl_optimizer.cpp",
				"<(root)/src/glsl/glsl_parser.cpp",
				"<(root)/src/glsl/glsl_parser_extras.cpp",
				"<(root)/src/glsl/glsl_symbol_table.cpp",
				"<(root)/src/glsl/glsl_types.cpp",
				"<(root)/src/glsl/hir_field_selection.cpp",
				"<(root)/src/glsl/ir_basic_block.cpp",
				"<(root)/src/glsl/ir_builder.cpp",
				"<(root)/src/glsl/ir_clone.cpp",
				"<(root)/src/glsl/ir_constant_expression.cpp",
				"<(root)/src/glsl/ir.cpp",
				"<(root)/src/glsl/ir_equals.cpp",
				"<(root)/src/glsl/ir_expression_flattening.cpp",
				"<(root)/src/glsl/ir_function_can_inline.cpp",
				"<(root)/src/glsl/ir_function.cpp",
				"<(root)/src/glsl/ir_function_detect_recursion.cpp",
				"<(root)/src/glsl/ir_hierarchical_visitor.cpp",
				"<(root)/src/glsl/ir_hv_accept.cpp",
				"<(root)/src/glsl/ir_import_prototypes.cpp",
				"<(root)/src/glsl/ir_print_glsl_visitor.cpp",
				"<(root)/src/glsl/ir_print_metal_visitor.cpp",
				"<(root)/src/glsl/ir_print_visitor.cpp",
				"<(root)/src/glsl/ir_rvalue_visitor.cpp",
				"<(root)/src/glsl/ir_stats.cpp",
				"<(root)/src/glsl/ir_unused_structs.cpp",
				"<(root)/src/glsl/ir_validate.cpp",
				"<(root)/src/glsl/ir_variable_refcount.cpp",
				"<(root)/src/glsl/link_atomics.cpp",
				"<(root)/src/glsl/linker.cpp",
				"<(root)/src/glsl/link_functions.cpp",
				"<(root)/src/glsl/link_interface_blocks.cpp",
				"<(root)/src/glsl/link_uniform_block_active_visitor.cpp",
				"<(root)/src/glsl/link_uniform_blocks.cpp",
				"<(root)/src/glsl/link_uniform_initializers.cpp",
				"<(root)/src/glsl/link_uniforms.cpp",
				"<(root)/src/glsl/link_varyings.cpp",
				"<(root)/src/glsl/loop_analysis.cpp",
				"<(root)/src/glsl/loop_controls.cpp",
				"<(root)/src/glsl/loop_unroll.cpp",
				"<(root)/src/glsl/lower_clip_distance.cpp",
				"<(root)/src/glsl/lower_discard.cpp",
				"<(root)/src/glsl/lower_discard_flow.cpp",
				"<(root)/src/glsl/lower_if_to_cond_assign.cpp",
				"<(root)/src/glsl/lower_instructions.cpp",
				"<(root)/src/glsl/lower_jumps.cpp",
				"<(root)/src/glsl/lower_mat_op_to_vec.cpp",
				"<(root)/src/glsl/lower_named_interface_blocks.cpp",
				"<(root)/src/glsl/lower_noise.cpp",
				"<(root)/src/glsl/lower_offset_array.cpp",
				"<(root)/src/glsl/lower_output_reads.cpp",
				"<(root)/src/glsl/lower_packed_varyings.cpp",
				"<(root)/src/glsl/lower_packing_builtins.cpp",
				"<(root)/src/glsl/lower_ubo_reference.cpp",
				"<(root)/src/glsl/lower_variable_index_to_cond_assign.cpp",
				"<(root)/src/glsl/lower_vec_index_to_cond_assign.cpp",
				"<(root)/src/glsl/lower_vec_index_to_swizzle.cpp",
				"<(root)/src/glsl/lower_vector.cpp",
				"<(root)/src/glsl/lower_vector_insert.cpp",
				"<(root)/src/glsl/lower_vertex_id.cpp",
				"<(root)/src/glsl/opt_algebraic.cpp",
				"<(root)/src/glsl/opt_array_splitting.cpp",
				"<(root)/src/glsl/opt_constant_folding.cpp",
				"<(root)/src/glsl/opt_constant_propagation.cpp",
				"<(root)/src/glsl/opt_constant_variable.cpp",
				"<(root)/src/glsl/opt_copy_propagation.cpp",
				"<(root)/src/glsl/opt_copy_propagation_elements.cpp",
				"<(root)/src/glsl/opt_cse.cpp",
				"<(root)/src/glsl/opt_dead_builtin_variables.cpp",
				"<(root)/src/glsl/opt_dead_builtin_varyings.cpp",
				"<(root)/src/glsl/opt_dead_code.cpp",
				"<(root)/src/glsl/opt_dead_code_local.cpp",
				"<(root)/src/glsl/opt_dead_functions.cpp",
				"<(root)/src/glsl/opt_flatten_nested_if_blocks.cpp",
				"<(root)/src/glsl/opt_flip_matrices.cpp",
				"<(root)/src/glsl/opt_function_inlining.cpp",
				"<(root)/src/glsl/opt_if_simplification.cpp",
				"<(root)/src/glsl/opt_minmax.cpp",
				"<(root)/src/glsl/opt_noop_swizzle.cpp",
				"<(root)/src/glsl/opt_rebalance_tree.cpp",
				"<(root)/src/glsl/opt_redundant_jumps.cpp",
				"<(root)/src/glsl/opt_structure_splitting.cpp",
				"<(root)/src/glsl/opt_swizzle_swizzle.cpp",
				"<(root)/src/glsl/opt_tree_grafting.cpp",
				"<(root)/src/glsl/opt_vectorize.cpp",
				"<(root)/src/glsl/s_expression.cpp",
				"<(root)/src/glsl/standalone_scaffolding.cpp",
				"<(root)/src/glsl/strtod.c"
			],
			"cflags!": [
				"-O3",
				"-g",
				'-fno-exceptions'
			],
			"cflags": [
				"-fpic",
				"-O2"
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x'
			],
			'cflags_cc': [
				'-std=c++11'
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"deps/glsl-optimizer/src"
				]
			}
		}
	]
}
