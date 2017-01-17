# GNU ARM Eclipse

The [GNU ARM Eclipse](http://gnuarmeclipse.github.io) is an open source project that includes a family of Eclipse plug-ins and tools for multi-platform embedded ARM development, based on GNU toolchains. The project is hosted on [GitHub](https://github.com/gnuarmeclipse).

This exporter aims to create managed GNU ARM Eclipse projects, ready to build, and with as many options properly identified and set in the Eclipse C/C++ Build -> Settings page.

## Build plug-in configuration options

All options are prefixed by `ilg.gnuarmeclipse.managedbuild.cross.option.`.

### Target Processor 

- `arm.target.family` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.mcpu.cortex-m3** '-mcpu=cortex-m3'
    - cortex-m0 '-mcpu=%s'
    - cortex-m0-small-multiply '-mcpu=%s'
    - cortex-m0plus '-mcpu=%s'
    - cortex-m0plus-small-multiply '-mcpu=%s'
    - cortex-m1 '-mcpu=%s'
    - cortex-m1-small-multiply '-mcpu=%s'
    - cortex-m4 '-mcpu=%s'
    - cortex-m7 '-mcpu=%s'

- `arm.target.architecture` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.arch.none** ''
    - armv6-m '-march=%s'
    - armv7-m '-march=%s'

- `arm.target.instructionset` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.instructionset.thumb** '-mthumb'
    - arm '-marm'
    
- `arm.target.thumbinterwork` bool
    - true `-mthumb-interwork`

- `arm.target.endianness` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.endianness.default**
    - little `-mlittle-endian`
    - big `-mbig-endian`

- `arm.target.fpu.abi` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.fpu.abi.default**
    - soft '-mfloat-abi=soft'
    - softfp '-mfloat-abi=softfp'
    - hard '-mfloat-abi=hard'

- `arm.target.fpu.unit` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.fpu.unit.default**
    - fpv4spd16 '-mfpu=fpv4-sp-d16'
    - fpv5d16 '-mfpu=fpv5-d16'
    - fpv5spd16 '-mfpu=fpv5-sp-d16'
    - ...

- `arm.target.unalignedaccess`
    - **ilg.gnuarmeclipse.managedbuild.cross.option.arm.target.unalignedaccess.default**
    - enabled '-munaligned-access'
    - disabled '-mno-unaligned-access'

- `target.other` string

### Optimization

- `optimization.level` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.optimization.level.more** '-O2'
    - none '-O0'
    - optimize '-O1'
    - most '-O3'
    - size '-Os'
    - debug '-Og'

- `optimization.messagelength` bool
    - **false** ''
    - true '-fmessage-length=0'

- `optimization.signedchar` bool
    - **false**
    - true '-fsigned-char'

- `optimization.functionsections` bool
    - **false**
    - true '-ffunction-sections'

- `optimization.datasections` bool
    - **false**
    - true '-fdata-sections'

- `optimization.nocommon` bool
    - **false**
    - true '-fno-common'

- `optimization.noinlinefunctions` bool
    - **false** ''
    - true '-fno-inline-functions'

- `optimization.freestanding` bool
    - **false** ''
    - true '-ffreestanding'

- `optimization.nobuiltin` bool
    - **false** ''
    - true '-fno-builtin'

- `optimization.spconstant` bool
    - **false** ''
    - true '-fsingle-precision-constant'

- `optimization.PIC` bool
    - **false** ''
    - true '-fPIC'

- `optimization.lto` bool
    - **false** ''
    - true '-flto'

- `optimization.nomoveloopinvariants` bool
    - **false** ''
    - true '-fno-move-loop-invariants'

- `optimization.other` string

- `` bool
    - **false** ''
    - true ''

### Warnings

- `warnings.syntaxonly` bool
    - **false** ''
    - true '-fsyntax-only'

- `warnings.pedantic` bool
    - **false** ''
    - true '-pedantic'

- `warnings.pedanticerrors` bool
    - **false** ''
    - true '-pedantic-errors'

- `warnings.nowarn` bool
    - **false** ''
    - true '-w'

- `warnings.unused` bool
    - **false** ''
    - true '-Wunused'

- `warnings.uninitialized` bool
    - **false** ''
    - true '-Wuninitialized'

- `warnings.allwarn` bool
    - **false** ''
    - true '-Wall'

- `warnings.extrawarn` bool
    - **false** ''
    - true '-Wextra'

- `warnings.missingdeclaration` bool
    - **false** ''
    - true '-Wmissing-declarations'

- `warnings.conversion` bool
    - **false** ''
    - true '-Wconversion'

- `warnings.pointerarith` bool
    - **false** ''
    - true '-Wpointer-arith'

- `warnings.padded` bool
    - **false** ''
    - true '-Wpadded'

- `warnings.shadow` bool
    - **false** ''
    - true '-Wshadow'

- `warnings.logicalop` bool
    - **false** ''
    - true '-Wlogical-op'

- `warnings.agreggatereturn` bool
    - **false** ''
    - true '-Waggregate-return'

- `warnings.floatequal` bool
    - **false** ''
    - true '-Wfloat-equal'

- `warnings.toerrors` bool
    - **false** ''
    - true '-Werror'

- `warnings.other` string

### Debugging

- `debugging.level` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.debugging.level.default** '-g'
    - none ''
    - minimal '-g1'
    - max '-g3'

- `debugging.format` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.debugging.format.default** ''
    - gdb '-ggdb'
    - stabs '-gstabs'
    - stabsplus '-gstabs+'
    - dwarf2 '-gdwarf-2'
    - dwarf3 '-gdwarf-3'
    - dwarf4 '-gdwarf-4'
    - dwarf5 '-gdwarf-5'

- `debugging.prof` bool
    - **false** ''
    - true '-p'

- `debugging.gprof` bool
    - **false** ''
    - true '-pg'

- `debugging.other` string


### Compiler common options

- `verbose` bool
    - **false** ''
    - true '-v'

- `savetemps` bool
    - **false** ''
    - true '--save-temps'

- `nostdinc` bool
    - **false** ''
    - true '-nostdinc'

- `asmlisting` bool
    - **false** ''
    - true '-Wa,-adhlns="$@.lst"'

- `preprocessor.preprocessonly` bool
    - **false** ''
    - true '-E'

- `dirs.include.files`
    - '-include%s'

- `compiler.other` string

### Linker common options

- `mapfilename` string
    - '-Wl,-Map,"${BuildArtifactFileBaseName}.map"'

- `linker.scriptfile`
    - '-T %s'

- `cref` bool
    - **false** ''
    - true '-Xlinker --cref'

- `printmap` bool
    - **false** ''
    - true '-Xlinker --print-map'

- `linker.nostart` bool
    - **false** ''
    - true '-nostartfiles'

- `linker.nodeflibs` bool
    - **false** ''
    - true '-nodefaultlibs'

- `linker.nostdlibs` bool
    - **false** ''
    - true '-nostdlib'

- `linker.gcsections` bool
    - **false** ''
    - true '-Xlinker --gc-sections'

- `linker.printgcsections` bool
    - **false** ''
    - true '-Xlinker --print-gc-sections'

- `linker.strip` bool
    - **false** ''
    - true '-s'

- `linker.other` string

- `linker.usenewlibnano` bool
    - **false** ''
    - true '--specs=nano.specs'

- `linker.useprintffloat` bool
    - **false** ''
    - true '-u \_printf\_float'

- `linker.usescanffloat` bool
    - **false** ''
    - true '-u \_scanf\_float'


### Cross ARM GNU Assembler

#### Preprocessor

- `nostdinc` bool
    - **false** ''
    - true '-nostdinc'

#### Includes

#### Warnings

#### Miscellaneous

- `verbose` bool
    - **false** ''
    - true '-v'

- `assembler.other` string

### Cross ARM GNU C Compiler

#### Preprocessor

- `nostdinc` bool
    - **false** ''
    - true '-nostdinc'

#### Includes

#### Optimization

- `c.compiler.std` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.c.compiler.std.default** ''
    - ansi '-ansi'
    - c90 '-std=c90' (c89)
    - gnu90 '-std=gnu90' (gnu89)
    - c99 '-std=c99' (c9x)
    - gnu99 '-std=gnu99' (gnu9x)
    - c11 '-std=c11' (c1x)
    - gnu11 '-std=gnu11' (gnu1x)

#### Warnings

- `c.compiler.warning.missingprototypes` bool
    - **false** ''
    - true '-Wmissing-prototypes'

- `c.compiler.warning.strictprototypes` bool
    - **false** ''
    - true '-Wstrict-prototypes'

- `c.compiler.warning.badfunctioncast` bool
    - **false** ''
    - true '-Wbad-function-cast'

#### Miscellaneous

- `verbose` bool
    - **false** ''
    - true '-v'

- `c.compiler.other` string

### Cross ARM GNU C++ Compiler

#### Preprocessor

- `nostdinc` bool
    - **false** ''
    - true '-nostdinc'

- `nostdincpp` bool
    - **false** ''
    - true '-nostdinc++'

#### Includes

#### Optimization

- `cpp.compiler.std` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.cpp.compiler.std.default** ''
    - ansi (-ansi)
    - cpp98 '-std=c++98' (c++03)
    - gnucpp98 '-std=gnu++98' (gnu++03)
    - cpp0x '-std=c++0x'
    - gnucpp0x '-std=gnu++0x'
    - cpp11 '-std=c++11'
    - gnucpp11 '-std=gnu++11'
    - cpp1y '-std=c++1y'
    - gnucpp1y '-std=gnu++1y'
    - cpp14 '-std=c++14'
    - gnucpp1 4'-std=gnu++14'
    - cpp1z '-std=c++1z'
    - gnucpp1z '-std=gnu++1z'

- `cpp.compiler.abiversion` enumerated
    - **ilg.gnuarmeclipse.managedbuild.cross.option.cpp.compiler.abiversion.0** '-fabi-version=0'
    - default ''
    - 1 '-fabi-version=1'
    - 2 '-fabi-version=2'
    - 3 '-fabi-version=3'
    - 4 '-fabi-version=4'
    - 5 '-fabi-version=5'
    - 6 '-fabi-version=6'

- `cpp.compiler.noexceptions` bool
    - **false** ''
    - true '-fno-exceptions'

- `cpp.compiler.nortti` bool
    - **false** ''
    - true '-fno-rtti'

- `cpp.compiler.nousecxaatexit` bool
    - **false** ''
    - true '-fno-use-cxa-atexit'

- `cpp.compiler.nothreadsafestatics` bool
    - **false** ''
    - true '-fno-threadsafe-statics'


#### Warnings

- `cpp.compiler.warnabi` bool
    - **false** '-Wabi'

- `cpp.compiler.warning.ctordtorprivacy` bool
    - **false** '-Wctor-dtor-privacy'

- `cpp.compiler.warning.noexcept` bool
    - **false** '-Wnoexcept'

- `cpp.compiler.warning.nonvirtualdtor` bool
    - **false** '-Wnon-virtual-dtor'

- `cpp.compiler.warning.strictnullsentinel` bool
    - **false** '-Wstrict-null-sentinel'

- `cpp.compiler.warning.signpromo` bool
    - **false** '-Wsign-promo'

- `cpp.compiler.warneffc` bool
    - **false** ''
    - true '-Weffc++'

#### Miscellaneous

- `verbose` bool
    - **false** ''
    - true '-v'

- `cpp.compiler.other` string

### Cross ARM GNU C++ Linker

#### General

- `cpp.linker.nostart`
    - **false** ''
    - true '-nostartfiles'

- `cpp.linker.nodeflibs`
    - **false** ''
    - true '-nodefaultlibs'

- `cpp.linker.nostdlibs`
    - **false** ''
    - true '-nostdlib'

#### Libraries

#### Miscellaneous


## Template

- `` bool
    - **false** ''
    - true ''
