import pfDevTools

pfDevTools.requires('1.0.12')

env = pfDevTools.SConsEnvironment(
    PF_CORE_TEMPLATE_REPO_TAG='v0.0.6_for_openFPGATutorials')

env.OpenFPGACore('src/config.toml',
                 extra_files=['src/assets/test.txt'])
