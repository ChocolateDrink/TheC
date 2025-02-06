using System.Text;
using System.Net.Http.Headers;
using System.Text.Json;

namespace AnimeUpdater
{
	internal class Program
	{
		private const int EXIT_FAILURE = 1;
		private const int EXIT_SUCCESS = 0;

		private const string REPO_NAME = "ChocolateDrink";
		private const string FILE_PATH = "misc/animes%20watched.md";
		private const string TOKEN = "";

		static async Task Main()
		{
			Console.ForegroundColor = ConsoleColor.White;

			if (string.IsNullOrWhiteSpace(TOKEN))
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("Github auth token is not set");
				Console.ForegroundColor = ConsoleColor.White;

				Environment.Exit(EXIT_FAILURE);
			}

			Console.Write("Enter anime name: ");

			string? anime = Console.ReadLine();
			if (String.IsNullOrWhiteSpace(anime))
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("Please provide an actual name");
				Console.ForegroundColor = ConsoleColor.White;

				Environment.Exit(EXIT_FAILURE);
			}

			anime = anime.Trim();

			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine($"Anime name: \"{anime}\"");
			Console.ForegroundColor = ConsoleColor.White;

			Console.WriteLine("yes/no");

			string? choice = Console.ReadLine();
			if (!String.IsNullOrWhiteSpace(choice) || choice.StartsWith('n'))
			{
				Console.Clear();
				await Main();
			}
			else
			{
				Console.Clear();
				await UpdateList(anime);
			}
		}

		static async Task UpdateList(string newAnime)
		{
			try
			{
				using (var client = new HttpClient())
				{
					client.DefaultRequestHeaders.Add("User-Agent", "AnimeUpdator");
					client.DefaultRequestHeaders.Add("Accept", "application/vnd.github+json");
					client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", TOKEN);

					string rawUrl = $"https://raw.githubusercontent.com/ChocolateDrink/{REPO_NAME}/refs/heads/main/{FILE_PATH}";
					string apiUrl = $"https://api.github.com/repos/ChocolateDrink/{REPO_NAME}/contents/{FILE_PATH}";

					string response = await client.GetStringAsync(rawUrl);
					string apiResponse = await client.GetStringAsync(apiUrl);

					var fileInfo = JsonSerializer.Deserialize<ApiThing>(apiResponse);

					List<string> lines = response.Split('\n').ToList();
					List<string> favs = [];
					List<string> all = [];
					string thisSection = "";

					foreach (string line in lines)
					{
						if (line.StartsWith("### "))
						{
							thisSection = line.Trim();
							continue;
						}

						if (!String.IsNullOrWhiteSpace(line))
						{
							string animeName = line.Trim();
							if (animeName.Contains(". "))
								animeName = animeName.Substring(animeName.IndexOf(". ") + 2);

							if (thisSection == "### Favorites:")
								favs.Add(animeName);
							else if (thisSection == "### Everything:")
								all.Add(animeName);
						}
					}

					if (!all.Contains(newAnime, StringComparer.OrdinalIgnoreCase))
					{
						all.Add(newAnime);
						all.Sort(StringComparer.OrdinalIgnoreCase);

						var builder = new StringBuilder();
						builder.AppendLine("### Favorites:");
						for (int i = 0; i < favs.Count; i++)
							builder.AppendLine($"{i + 1}. {favs[i]}");

						builder.AppendLine();
						builder.AppendLine("### Everything:");
						for (int i = 0; i < all.Count; i++)
							builder.AppendLine($"{i + 1}. {all[i]}");

						var request = new
						{
							message = $"Added anime: {newAnime}",
							content = Convert.ToBase64String(Encoding.UTF8.GetBytes(builder.ToString())),
							sha = fileInfo?.sha
						};

						var requestJson = JsonSerializer.Serialize(request);
						var requestContent = new StringContent(requestJson, Encoding.UTF8, "application/json");

						var commitResponse = await client.PutAsync(apiUrl, requestContent);
						if (commitResponse.IsSuccessStatusCode)
						{
							Console.ForegroundColor = ConsoleColor.Green;
							Console.WriteLine($"Successfully updated GitHub with \"{newAnime}\"");
							Console.ForegroundColor = ConsoleColor.White;

							Environment.Exit(EXIT_SUCCESS);
						}
						else
						{
							Console.ForegroundColor = ConsoleColor.Red;
							Console.WriteLine($"Failed to update GitHub: {commitResponse.StatusCode} - {await commitResponse.Content.ReadAsStringAsync()}");
							Console.ForegroundColor = ConsoleColor.White;

							Environment.Exit(EXIT_FAILURE);
						}
					}
					else
					{
						Console.ForegroundColor = ConsoleColor.Yellow;
						Console.WriteLine($"\"{newAnime}\" is already in the list");
						Console.ForegroundColor = ConsoleColor.White;

						Environment.Exit(EXIT_FAILURE);
					}
				}
			}
			catch (HttpRequestException ex)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine($"Http Error: {ex.Message}");
				Console.ForegroundColor = ConsoleColor.White;

				Environment.Exit(EXIT_FAILURE);
			}
			catch (Exception ex)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine($"Unexpected Error {ex.Message}");
				Console.ForegroundColor = ConsoleColor.White;

				Environment.Exit(EXIT_FAILURE);
			}
		}

		class ApiThing
		{
			public string? sha { get; set; }
		}
	}
}
