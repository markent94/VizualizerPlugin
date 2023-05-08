using UnrealBuildTool;

public class CSVDataVisualizer : ModuleRules {
    public CSVDataVisualizer ( ReadOnlyTargetRules Target ) : base ( Target ) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange ( new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd" } );

        PrivateDependencyModuleNames.AddRange ( new string[] {  } );

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange ( new string[] { "Slate", "SlateCore", "DesktopPlatform" } );

        // Uncomment if you are using UObject or UActorComponent derived classes for your plugin
        PrivateIncludePaths.AddRange ( new string[] { "CSVDataVisualizer/Private" } );
    }
}
