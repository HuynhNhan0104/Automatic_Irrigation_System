pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven {
            setUrl("https://repo.eclipse.org/content/repositories/paho-snapshots/")
            setUrl("https://jitpack.io")
        }
    }
}


rootProject.name = "IOT_APP"
include(":app")
