project(pipes LANGUAGES CXX)

cs_add_library(pipes INTERFACE)
cs_add_library(csCore::pipes ALIAS pipes)
cs_exec_clang_tidy(pipes)

cs_find_package(pipes PUBLIC pipes REQUIRED CONFIG)

cs_target_sources(pipes
	PRIVATE_INSTALL
		PipesCore.hpp
	# copy of list.txt
		drop_while.hpp
		fork.hpp
		send.hpp
		tap.hpp
		stride.hpp
		to_out_stream.hpp
		filter.hpp
		pipes.hpp
		set_aggregator.hpp
		map_aggregator.hpp
		drop.hpp
		combinations.hpp
		partition.hpp
		unzip.hpp
		push_back.hpp
		operator.hpp
		take_while.hpp
		tee.hpp
		transform.hpp
		take.hpp
		switch.hpp
		read_in_stream.hpp
		dev_null.hpp
		intersperse.hpp
		adjacent.hpp
		insert.hpp
		mux.hpp
		base.hpp
		join.hpp
		cartesian_product.hpp
		for_each.hpp
		override.hpp
		do_then.hpp
		helpers/assignable.hpp
		helpers/optional.hpp
		helpers/FWD.hpp
		helpers/meta.hpp
		helpers/detect.hpp
		helpers/invoke.hpp
		helpers/crtp.hpp
		impl/concepts.hpp
		impl/pipes_assembly.hpp
	PRIVATE
    )

cs_target_link_libraries(pipes
	INTERFACE
		csCore::csCore
		pipes::pipes
	)

add_subdirectory(tests)

cs_install_target(pipes pipes)

cs_finalize_target(pipes)

# compile each single header in standalone, ensures includes are self contained
cs_compileEachInstalledInclude(pipes csCoreSol::csCoreSol_csCMakeCompileOptionsStrict)
