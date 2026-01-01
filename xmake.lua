add_rules("mode.debug", "mode.release")
set_languages("c++20")
basic_libs = {
    "nlohmann_json"
}
vendored_libs = {
    "pkgconfig::sdl3",
    "pkgconfig::sdl3-image",
    "pkgconfig::sdl3-ttf"
}

for _, libname in ipairs(basic_libs) do
    add_requires(libname)
end
for _, libname in ipairs(vendored_libs) do
    add_requires(libname, {system = true})
end
target("deltarune_cnpatcher")
    set_kind("binary")

    -- source files
    add_files("src/main.cpp")
    add_files("src/**/*.cpp")

    -- packages
    add_packages(table.unpack(basic_libs))
    add_packages(table.unpack(vendored_libs))
    -- defines
    add_defines("UNICODE", "_UNICODE")

    before_build(function (target)
        -- clean target output directory before each build
        local outdir = target:targetdir()
        if outdir and os.isdir(outdir) then
            os.rmdir(outdir)
        end
    end)

    if is_plat("windows") and is_mode("release") then
        add_files("app/win.rc")
     --   add_ldflags("/SUBSYSTEM:WINDOWS");
    end

    after_build(function (target)
        local outdir = target:targetdir()
        os.cp(path.join(os.projectdir(), "external"), outdir)
        os.cp(path.join(os.projectdir(), "image"), outdir)
        os.cp(path.join(os.projectdir(), "lang"), outdir)
        os.cp(path.join(os.projectdir(), "font"), outdir)
        os.cp(path.join(os.projectdir(), "readme*.txt"), outdir)
        os.cp(path.join(os.projectdir(), "dlls/*.dll"), outdir)
    end)
