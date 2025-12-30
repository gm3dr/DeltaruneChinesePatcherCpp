add_requires(
    "cpr",
    "nlohmann_json",
    "picosha2"
)
add_rules("mode.debug", "mode.release")
set_languages("c++20")

local vendored_libs = {
    "pkgconfig::sdl3", 
    "pkgconfig::sdl3-image", 
    "pkgconfig::sdl3-ttf", 
    "pkgconfig::sdl3-mixer"
}

for _, libname in ipairs(vendored_libs) do
    add_requires(libname, {system = true})
end
target("deltarune_cnpatcher")
    set_kind("binary")

    -- source files
    add_files("src/main.cpp")
    add_files("src/**/*.cpp")

    -- packages
    add_packages(
        "cpr",
        "nlohmann_json",
        "picosha2"
    )
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
        add_ldflags("/SUBSYSTEM:WINDOWS");
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
